#include "gameloop.hpp"
#include "main_helper.hpp"
#include "Player.hpp"
#include "chat.hpp"
#include "worldObjects.hpp"
#include "Building.hpp"
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


    WorldObject objs[2];
    WorldObject horse("Art/Decor/Horse.png", renderer, 2000, 1000, 200,100, 2020, 1010, 160, 80);
    Collidable c(2100, 1010, 35, 40);
    horse.addChild(c);
    Collidable c1(2020, 1050, 160, 40);
    horse.addChild(c1);
    objs[0] = horse;
    objs[1].initObject("Art/Decor/Bed.png", renderer, 2000, 1500, 100, 100, 2000, 1500, 100, 100);

    Building b("Art/Buildings/generic.png", renderer, 2500, 1200, 500, 500, 2500, 1200, 500, 500);
    SDL_Rect door = {200,121, 150, 179};
    b.addWalls(20, door, WEST);
    //display npcs
    NPC npcs[12];
    // lust, loyal, wrath => green, blue, red
    npcs[0].initSprite("Benedict", "Art/NPCs/Blacksmith.bmp", BROWN, GRAY, BLACK,
                        renderer, 60, 88, 1400, 1000);
    npcs[1].initSprite("Charles", "Art/NPCs/Vicar.bmp", WHITE, WHITE, PURPLE,
                        renderer, 60, 88, 1600, 1000);
    npcs[2].initSprite("David", "Art/NPCs/Worker1.bmp", ORANGE, DARK_BLUE, WHITE,
                        renderer, 60, 88, 1800, 1000);
    npcs[3].initSprite("Erick", "Art/NPCs/Worker2.bmp", ORANGE, LIGHT_BLUE, WHITE,
                        renderer, 60, 88, 1400, 1200);
    npcs[4].initSprite("Frank", "Art/NPCs/Worker3.bmp", ORANGE, BLACK, WHITE,
                        renderer, 60, 88, 1600, 1200);
    npcs[5].initSprite("Gail", "Art/NPCs/Worker4.bmp", ORANGE, LIGHT_BLUE, WHITE,
                        renderer, 60, 88, 1800, 1200);
    npcs[6].initSprite("Henry", "Art/NPCs/Mayor.bmp", WHITE, GRAY, DARK_GREEN,
                        renderer, 60, 88, 1400, 1400);
    npcs[7].initSprite("Isaac", "Art/NPCs/Fuzz.bmp", WHITE, DARK_BLUE, ORANGE,
                        renderer, 60, 88, 1600, 1400);
    npcs[8].initSprite("Jake", "Art/NPCs/Barkeep.bmp", LIGHT_RED, GRAY, WHITE,
                        renderer, 60, 88, 2600, 1550);
    npcs[9].initSprite("Kyle", "Art/NPCs/Farmer.bmp", LIGHT_GREEN, BLUE, ORANGE,
                        renderer, 60, 88, 1400, 1600);
    npcs[10].initSprite("Liam", "Art/NPCs/Jeweler.bmp", WHITE, GRAY, BLACK,
                        renderer, 60, 88, 1600, 1600);
    npcs[11].initSprite("Michael", "Art/NPCs/FishMonger.bmp", GREEN, DARK_BLUE, WHITE,
                        renderer, 60, 88, 1400, 1800);
    
    

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

        SDL_Rect collide;
        // render buildings
        b.renderToScreen(renderer, cam);
        if (b.checkCollision(&(player->positionPNG), &collide)){
                player->alterPosition(&collide);
        }
        //render npc
        int i=0;
        for(i = 0; i < 12; i++){
            npcs[i].renderToScreen(renderer, time_change, cam);
            if( SDL_IntersectRect(&npcs[i].mapPos, &(player->positionPNG), &collide)){
                //std::cout << "Player collision with " << npcs[i].getName() << std::endl;
                    player->alterPosition(&collide);
            }
        }
        //printf("Object size: %d\n", objs.size());
        for (int i = 0; i < 2; i++){
            objs[i].renderToScreen(renderer, cam);
            if (objs[i].checkCollision(&(player->positionPNG), &collide)){
                player->alterPosition(&collide);
            }
        }
        
        
        setCameraPosition(&cam, player->positionPNG);
        
        // Render Player
        player->render(renderer, &cam);

        // Check Collisions
        player->collision(renderer, keyState);

        //Update Screen
        SDL_RenderPresent(renderer);
    }
}
