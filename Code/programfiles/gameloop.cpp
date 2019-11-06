#include "gameloop.hpp"
#include "main_helper.hpp"
#include "Player.hpp"
#include "chat.hpp"
#include "worldObjects.hpp"
#include "Building.hpp"
#include "../NPC_Gen/npc.hpp"

#include <time.h>

// Add some vars to be used below
std::string mapImgPath = "Art/Tiles/TownMap.png";
std::string playerImgPath = "Art/Player/PlayerSpriteSheet.png";
const int NPC_NUM = 12;
const int WORLD_OBJECT_NUM = 6;

const int NPC_WIDTH = 60;
const int NPC_HEIGHT = 88;

void init(NPC *npcs, SDL_Renderer *renderer){
  // lust, loyal, wrath => green, blue, red
  // MarketPeople at the Market
  npcs[0].initSprite("Benedict", "Art/NPCs/Blacksmith.bmp", BROWN, GRAY, BLACK,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 1600, 1250);
  npcs[1].initSprite("Liam", "Art/NPCs/Jeweler.bmp", WHITE, GRAY, BLACK,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 1900, 1250);
  npcs[2].initSprite("Michael", "Art/NPCs/FishMonger.bmp", GREEN, DARK_BLUE, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 1600, 1550);
  npcs[3].initSprite("Kyle", "Art/NPCs/Farmer.bmp", LIGHT_GREEN, BLUE, ORANGE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 1900, 1550);

  // Workers in the factory
  npcs[4].initSprite("David", "Art/NPCs/Worker1.bmp", ORANGE, DARK_BLUE, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 2900, 400);
  npcs[5].initSprite("Erick", "Art/NPCs/Worker2.bmp", ORANGE, LIGHT_BLUE, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 3400, 400);
  npcs[6].initSprite("Frank", "Art/NPCs/Worker3.bmp", ORANGE, BLACK, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 2900, 700);
  npcs[7].initSprite("Gail", "Art/NPCs/Worker4.bmp", ORANGE, LIGHT_BLUE, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 3400, 700);

  // Administrators in the town hall
  npcs[8].initSprite("Henry", "Art/NPCs/Mayor.bmp", WHITE, GRAY, DARK_GREEN,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 250, 500);
  npcs[9].initSprite("Isaac", "Art/NPCs/Fuzz.bmp", WHITE, DARK_BLUE, ORANGE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 600, 500);

  // Barkeep at the bar
  npcs[10].initSprite("Jake", "Art/NPCs/Barkeep.bmp", LIGHT_RED, GRAY, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 400, 1450);

  // Church Person at the Church
  npcs[11].initSprite("Charles", "Art/NPCs/Vicar.bmp", WHITE, WHITE, PURPLE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 3500, 1600);

  // Make one of them a ghost
  // TODO: During integration, remove this bit and replace with call to sim
  // Also don't forget to get rid of the time.h import
  srand(time(NULL));
  int random = rand() % 12;
  npcs[random].ghostThisNPC();
}

void gameloop(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, bool farnan){
    // Initialize world texture, player texture, and camera
    SDL_Texture *bg = loadFiles(mapImgPath, renderer);
    Player *player = new Player(playerImgPath, renderer);
    SDL_Rect cam = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    // Create a building, a bed, and a horse
    WorldObject objs[WORLD_OBJECT_NUM];
    objs[0].initObject("Art/Decor/Horse.png", renderer, 2000, 1000, 200, 100, 10, 10, 180, 80);
    
    objs[1].initObject("Art/Decor/TealBlock.png", renderer, 1297, 540, 485, 55, 15, 15, 440, 45);
    
    objs[2].initObject("Art/Decor/TealBlock.png", renderer, 1297, 35, 59, 585, 10, 10, 40, 540);
    
    objs[3].initObject("Art/Decor/TealBlock.png", renderer, 1294, 35, 1070, 58, 15, 15, 1000, 35);
    
    objs[4].initObject("Art/Decor/TealBlock.png", renderer, 2265, 35, 59, 585, 10, 10, 40, 540);
    
    objs[5].initObject("Art/Decor/TealBlock.png", renderer, 1855, 540, 485, 55, 40, 15, 410, 45);

    // Create the NPCs (offloaded for brevity)
    NPC npcs[NPC_NUM];
    init(npcs, renderer);

    // Used for framerate independence
    int curr_time = 0;
    int last_time = 0;
    float time_change;

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
        //render npcs
        int i=0;
        for(i = 0; i < NPC_NUM; i++){
            npcs[i].renderToScreen(renderer, time_change, cam);
            if( SDL_IntersectRect(&npcs[i].mapPos, &(player->positionPNG), &collide)){
                    player->alterPosition(&collide);
            }
        }

        for (int i = 0; i < WORLD_OBJECT_NUM; i++){
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
