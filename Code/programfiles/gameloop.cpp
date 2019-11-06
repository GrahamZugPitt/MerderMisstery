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
std::string interactImgPath = "Art/Messages/interact.png";
const int NPC_NUM = 12;
const int WORLD_OBJECT_NUM = 2;

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

void renderTexture(SDL_Renderer *renderer, SDL_Texture *texture, SDL_Rect cam, int x, int y, int w, int h, bool useCam){
  // Render the 'Press whatever to talk' button
  SDL_Rect screenPos;
  screenPos.x = x;
  screenPos.y = y;
  screenPos.w = w;
  screenPos.h = h;

  if(useCam)
    SDL_RenderCopy(renderer, texture, &cam, &screenPos);
  else
    SDL_RenderCopy(renderer, texture, NULL, &screenPos);
}

void gameloop(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, bool farnan){
    // Initialize world texture, player texture, and camera
    SDL_Texture *bg = loadFiles(mapImgPath, renderer);
    SDL_Texture *interactPromptingTex = loadFiles(interactImgPath, renderer);
    Player *player = new Player(playerImgPath, renderer);
    SDL_Rect cam = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    // Create a horse
    WorldObject objs[WORLD_OBJECT_NUM];
    objs[0].initObject("Art/Decor/Horse.png", renderer, 2000, 1000, 200, 100, 25, 10, 160, 45);

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
        renderTexture(renderer, bg, cam, 0, 0, cam.w, cam.h, true);

        SDL_Rect collide;
        //render npcs and check for NPC collisions
        //  also check if you're within the conversation fields
        int i=0;
        for(i = 0; i < NPC_NUM; i++){
            npcs[i].renderToScreen(renderer, time_change, cam);
            if (npcs[i].NPCCollider.checkCollision(&(player->positionPNG), &collide)){
                player->alterPosition(&collide);
            }

            if (npcs[i].NPCConversationCollider.checkCollision(&(player->positionPNG), &collide)){
                printf("Load her up, boys!\n");
                int w, h;
                SDL_QueryTexture(interactPromptingTex, NULL, NULL, &w, &h);
                SDL_Rect notreally;
                renderTexture(renderer, interactPromptingTex, notreally, 10, 10, w, h, false);
            }
        }

        // render world objects and check for collisions
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
