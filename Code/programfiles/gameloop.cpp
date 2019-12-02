#include "gameloop.hpp"
#include "main_helper.hpp"
#include "Player.hpp"
#include "chat.hpp"
#include "inventory.hpp"
#include "discussion.hpp"
#include "worldObjects.hpp"
#include "Building.hpp"
#include "../NPC_Gen/npc.hpp"
#include "Simulation/simulation.hpp"

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

// Return the seed so we can use it later if necessary
std::string simTown(NPClite *town, std::string seed){
  int seedint;
  // Convert the seed from a string to an int
  if(seed.compare("") != 0){ // If you don't have one, we'll just use the time
    seedint = time(0);
  }
  else{
    // TODO: Set seedint with some numeric version of the passed in string
    //    I need to get to bed
  }

  // Dunno if it has to be positive, but may as well.
  if(seedint < 1) seedint = (-1 * seedint) + 1;
  // There's literally only one case where this'd be an issue, so yannow
  if(seedint < 1) seedint = 1;

  int goodKill = 0;
	while(goodKill == 0){
		NPClite jarrett("Benedict", MERCHANT); //0
		NPClite kim("Liam", MERCHANT); //1
		NPClite pope("Micheal", MERCHANT); //2
		NPClite gaben("Kyle", MERCHANT); //3
		NPClite marie("David", WORKER); //4
		NPClite lary("Erick", WORKER); //5
		NPClite luigi("Frank", WORKER); //6
		NPClite albert("Gail", WORKER); //7
		NPClite dennis("Henry", MAYOR); //8
		NPClite helen("Isaac", POLICE); //9
		NPClite merge("Jake", INNKEEPER); //10
		NPClite sigmund("Charles", PRIEST); //11
		town[0] = jarrett;
		town[1] = kim;
		town[2] = pope;
		town[3] = gaben;
		town[4] = marie;
		town[5] = lary;
		town[6] = luigi;
		town[7] = albert;
		town[8] = dennis;
		town[9] = helen;
		town[10] = merge;
		town[11] = sigmund;
	  simulation(town, seedint);
    if(goodMurder(town)) goodKill = 1;
	}
}

// Has to return the seed because we'll need it later
std::string init(NPC *npcs, SDL_Renderer *renderer, std::string seed, NPClite *town){
  // Runs the simulation. I know, it looks so innocuous. Kinda nifty, right?
  seed = simTown(town, seed);

  // Set up the front end NPCs
  //   Presumably this is where the backend info will be pushed to the front end
  // lust, loyal, wrath => green, blue, red
  // MarketPeople at the Market
  npcs[0].initSprite("Benedict", "Art/NPCs/Blacksmith.bmp", BROWN, GRAY, BLACK,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 1796, 1108);
  npcs[1].initSprite("Liam", "Art/NPCs/Jeweler.bmp", WHITE, GRAY, BLACK,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 1866, 1108);
  npcs[2].initSprite("Michael", "Art/NPCs/FishMonger.bmp", GREEN, DARK_BLUE, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 1936, 1108);
  npcs[3].initSprite("Kyle", "Art/NPCs/Farmer.bmp", LIGHT_GREEN, BLUE, ORANGE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 2006, 1108);

  // Workers in the factory
  npcs[4].initSprite("David", "Art/NPCs/Worker1.bmp", ORANGE, DARK_BLUE, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 2420, 748);
  npcs[5].initSprite("Erick", "Art/NPCs/Worker2.bmp", ORANGE, LIGHT_BLUE, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 2490, 748);
  npcs[6].initSprite("Frank", "Art/NPCs/Worker3.bmp", ORANGE, BLACK, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 2560, 748);
  npcs[7].initSprite("Gail", "Art/NPCs/Worker4.bmp", ORANGE, LIGHT_BLUE, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 2630, 748);

  // Administrators in the town hall
  npcs[8].initSprite("Henry", "Art/NPCs/Mayor.bmp", WHITE, GRAY, DARK_GREEN,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 990, 482);
  npcs[9].initSprite("Isaac", "Art/NPCs/Fuzz.bmp", WHITE, DARK_BLUE, ORANGE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 1358, 685);

  // Barkeep at the bar
  npcs[10].initSprite("Jake", "Art/NPCs/Barkeep.bmp", LIGHT_RED, GRAY, WHITE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 1431, 1452);

  // Church Person at the Church
  npcs[11].initSprite("Charles", "Art/NPCs/Vicar.bmp", WHITE, WHITE, PURPLE,
                      renderer, NPC_WIDTH, NPC_HEIGHT, 2248, 1670);

  // Make one of them a ghost
  int i;
  for(i = 0; i < 12; i++){
    if(town[i].isDead){
      npcs[i].ghostThisNPC();
    }
  }
}

void gameloop(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, bool farnan, std::string seed){
    // Initialize world texture, player texture, and camera
    SDL_Texture *bg = loadFiles(mapImgPath, renderer);
    SDL_Texture *interactPromptingTex = loadFiles(interactImgPath, renderer);
    Player *player = new Player(playerImgPath, renderer);
    SDL_Rect cam = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    NPClite *town = NPClite[12];


    // Create the NPCs (offloaded for brevity)
    NPC npcs[NPC_NUM];
    init(npcs, renderer, seed);

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

        // Open Chat room
        if (keyState[SDL_SCANCODE_C])
            enter_chat(e, &(*quit), keyState, renderer);

        // Open the inventory
        if (keyState[SDL_SCANCODE_I])
            open_inventory(e, &(*quit), keyState, renderer);

        //Talk to an NPC
        if (keyState[SDL_SCANCODE_X] && discussbool)
            enter_discussion(e, &(*quit), keyState, renderer, &(npcs[npcdiscuss]));

        // Quit may have changed during the dialogue, so it's best to check
        if (*quit)
            return;

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
            player->alterPosition(&collide);
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
