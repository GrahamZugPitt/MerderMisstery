#include "gameloop.hpp"
#include "main_helper.hpp"
#include "Player.hpp"
#include "chat.hpp"
#include "discussion.hpp"
#include "worldObjects.hpp"
#include "Building.hpp"
#include "../NPC_Gen/npc.hpp"

#include "CyanBuilding.hpp"

#include "BlueBuilding.hpp"
#include "GreenBuilding.hpp"
#include "YellowBuilding.hpp"
#include "RedBuilding.hpp"

#include <time.h>

// Add some vars to be used below
std::string mapImgPath = "Art/MapSamples/SampleMap.png";
std::string playerImgPath = "Art/Player/PlayerSpriteSheet.png";
std::string interactImgPath = "Art/Messages/interact.png";
const int NPC_NUM = 12;
const int WORLD_OBJECT_NUM = 2;

//NPC Constants
const int NPC_WIDTH = 60;
const int NPC_HEIGHT = 88;

int npcdiscuss = 0;
bool discussbool = false;

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
    SDL_Texture *interactPromptingTex = loadFiles(interactImgPath, renderer);
    Player *player = new Player(playerImgPath, renderer);
    SDL_Rect cam = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};


    // Create the NPCs (offloaded for brevity)
    NPC npcs[NPC_NUM];
    init(npcs, renderer);

    // Used for framerate independence
    int curr_time = 0;
    int last_time = 0;
    float time_change;
    int frames_rendered = 0;
    int fr_timer = 0;
    
    // Allocate buildings
    CyanBuilding cBuilding;   // West (Town Hall)
    BlueBuilding bBuilding;   // East (Factory)
    GreenBuilding gBuilding;  // South East (Church)
    YellowBuilding yBuilding; // South West (Residences?)
    RedBuilding rBuilding;    // North  (Murder *Ominous music playing*)
  //PurpleBuilding pBuilding; // Central (Courtyard)

    // Collision dectction variables
    SDL_Rect collide;
    SDL_Rect convCollide;
    int hasCollided = 0;
    //Enter Game Loop
    while(!(*quit)) {
        //SDL time and delta value
        last_time = curr_time;
        curr_time = SDL_GetTicks();
        time_change = (curr_time - last_time) / 500.0f;
        frames_rendered++;
        fr_timer += (curr_time - last_time);
        if(fr_timer >= 1000){
          std::cout << "fps: " << frames_rendered << std::endl;
          fr_timer = 0;
          frames_rendered = 0;
        }
        hasCollided = 0;
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

        //Open Chat room
        if (keyState[SDL_SCANCODE_X] && discussbool)
            enter_discussion(e, &(*quit), keyState, renderer);

        //Move Player
        player->move(time_change, keyState, farnan);

        //Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Renders the background
        renderTexture(renderer, bg, cam, 0, 0, cam.w, cam.h, true);

        
        //render npcs
        int i=0;
        int indiscusscollider = 0;

        // check for collisions
        // Loop through twice to accomidate corner sections where the player may collide with 2 objects
        for (int axisI = 0; axisI < 2; axisI++){
          // Check NPC Collisions
          for(i = 0; i < NPC_NUM; i++){
              if (npcs[i].NPCCollider.checkCollision(&(player->positionPNG), &collide)){
                  hasCollided = 2;
              }
          }
          //Check building collisions
          if (cBuilding.checkCollision(&(player->positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (bBuilding.checkCollision(&(player->positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (gBuilding.checkCollision(&(player->positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (yBuilding.checkCollision(&(player->positionPNG), &collide)) {
              hasCollided = 3;
          }
          if (rBuilding.checkCollision(&(player->positionPNG), &collide)) {
              hasCollided = 3;
          }
          if(hasCollided){
            player->alterHorizPosition(&collide);
          } else {
            axisI = 2;
          }
          hasCollided = 0;
        }

        //render NPC's and check for conversation
        for(i = 0; i < NPC_NUM; i++){
            npcs[i].renderToScreen(renderer, time_change, cam);
            if (npcs[i].NPCConversationCollider.checkCollision(&(player->positionPNG), &convCollide)){
                int w, h;
                SDL_QueryTexture(interactPromptingTex, NULL, NULL, &w, &h);
                // Just pass in collide as a dummy arg, we don't use it
                renderTexture(renderer, interactPromptingTex, collide, 10, 10, w, h, false);
                npcdiscuss = i;
                indiscusscollider = true;
            }
        }
        // Only can discuss if we're within range
        discussbool = indiscusscollider;

        setCameraPosition(&cam, player->positionPNG);

        // Render Player
        player->render(renderer, &cam);

        // Check Collisions
        player->collision(renderer, keyState);

        //Update Screen
        SDL_RenderPresent(renderer);
    }
}
