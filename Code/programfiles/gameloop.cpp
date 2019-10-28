#include "gameloop.hpp"
#include "main_helper.hpp"
#include "Player.hpp"
#include "chat.hpp"
#include "../NPC_Gen/npc.hpp"
// Add some vars to be used below
std::string mapImgPath = "Art/Tiles/Map.png";
std::string playerImgPath = "Art/Player/PlayerSpriteSheet.png";

void gameloop(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, bool farnan){
    // Set variables
    int curr_time = 0;
    int last_time = 0;
    float time_change;
    SDL_Texture* bg = loadFiles(mapImgPath, renderer);
    Player *player = new Player(playerImgPath, renderer);
    SDL_Rect cam = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    //display an npc
    #ifndef NPCS_AS_VECTOR
    NPC npcs[6];
    
    npcs[0].initSprite("Benedict", "Art/NPCs/Blacksmith.bmp", BROWN, GRAY, BLACK,
                        renderer, 60, 88, 1400, 1000);
    npcs[1].initSprite("Charles", "Art/NPCs/Vicar.bmp", WHITE, WHITE, PURPLE,
                        renderer, 60, 88, 1600, 1000);
    npcs[2].initSprite("David", "Art/NPCs/Worker1.bmp", ORANGE, DARK_BLUE, WHITE,
                        renderer, 60, 88, 1800, 1000);
    npcs[3].initSprite("Erick", "Art/NPCs/Worker2.bmp", ORANGE, LIGHT_BLUE, WHITE,
                        renderer, 60, 88, 1400, 1200);
    npcs[4].initSprite("Frank", "Art/NPCs/Worker3.bmp", ORANGE, BLACK, WHITE,
                        renderer, 60, 88, 1600, 1200);//npcs[3].initSprite(renderer, 60, 88, 3000, 4100);
    npcs[5].initSprite("Gail", "Art/NPCs/Worker4.bmp", ORANGE, LIGHT_BLUE, WHITE,
                        renderer, 60, 88, 1800, 1200);//npcs[4].initSprite(renderer, 60, 88, 3250, 4000);
    //npcs[5].initSprite(renderer, 60, 88, 3250, 4100);

    #else
    NPC blm("Benedict", "Art/NPCs/Blacksmith.bmp", 0xFF000000, 0xFF000000, 0xFF888888);
    blm.initSprite(renderer, 60, 88, 500, 1000);
    #endif

    //Enter Game Loop
    while(!(*quit)) {
        //SDL time and delta value
        last_time = curr_time;
        curr_time = SDL_GetTicks();
        time_change = (curr_time - last_time) / 500.0f;

        //Really would rather export this to an event handling file
        while(SDL_PollEvent(&e) != 0)
        {
            //Quit application
            if(e.type == SDL_QUIT)
                (*quit) = true;
        }

        // Get the Keyboard State
        keyState = SDL_GetKeyboardState(NULL);

        //Open Chat room
        if (keyState[SDL_SCANCODE_C])
            enter_chat(e, &(*quit), keyState, renderer);

        //Move Player
        player->move(time_change, keyState, farnan);

        setCameraPosition(&cam, player->positionPNG);

        //Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Renders the background
        SDL_Rect bgRect;
        bgRect.x = 0;
        bgRect.y = 0;
        bgRect.w = cam.w;
        bgRect.h = cam.h;
        SDL_RenderCopy(renderer, bg, &cam, &bgRect);

        //render npc

        int i=0;
        SDL_Rect collide;
        for(i = 0; i < 6; i++){
            npcs[i].renderToScreen(renderer, time_change, cam);
            if( SDL_IntersectRect(&npcs[i].mapPos, &(player->positionPNG), &collide)){
                std::cout << "Player collision with " << npcs[i].getName() << std::endl;
            }
        }

        // Render Player
        player->render(renderer, &cam);

        // Check Collisions
        player->collision(renderer, keyState);

        //Update Screen
        SDL_RenderPresent(renderer);
    }
}
