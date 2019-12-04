#ifndef __GAMELOOPCPP__
#define __GAMELOOPCPP__

#include "gameloop.hpp"
#include <iostream>

// Add some vars to be used below
std::string mapImgPath = "Art/MapSamples/SampleMap.png";
std::string playerImgPath = "Art/Player/PlayerSpriteSheet.png";
std::string interactImgPath = "Art/Messages/interact.png";
const int NPC_NUM = 12;
const int WORLD_OBJECT_NUM = 4;
const int PERSONALITY_TRAITS = 5;

//NPC Constants
const int NPC_WIDTH = 60;
const int NPC_HEIGHT = 88;

int npcdiscuss = 0;
bool discussbool = false;

NPClite* npcToNPClite(NPClite* town, NPC* npcs){
	for(int i = 0; i < NPC_NUM; i++){
		town[i].name = npcs[i].getName();
		for(int j = 0; j < PERSONALITY_TRAITS; j++){
			town[i].personality[j] = (npcs[i].getPersonality(j) + 100)/2; 
		}
	}
	return town;
}

// Return the seed so we can use it later if necessary
std::string simTown(NPClite *town, NPC* npcs, std::string seed){
  int seedint = -1;

  if(seed.compare("") == 0){ // If you don't have one, we'll just use the time
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
    std::string time_str(buffer);

    seed = time_str;
  }

  const char *seedCharray = seed.c_str();
  int i;
  for(i = 0; i < seed.length(); i++){
    seedint = ((seedCharray[i] * seedint) % M) + 1;
  }

  std::cout << "Your seed is: " << seed << std::endl;

  // Dunno if it has to be positive, but may as well.
  if(seedint < 1) seedint = (-1 * seedint) + 1;
  // There's literally only one case where this'd be an issue, so yannow
  if(seedint < 1) seedint = 1;

  int goodKill = 0;
	while(goodKill == 0){
		NPClite jarrett("Benefffffffffffdict", MERCHANT); //0
		NPClite kim("Ligggggggggggam", MERCHANT); //1
		NPClite pope("Micggggggggggggggggheal", MERCHANT); //2gg
		NPClite gaben("Kggggggggggyle", MERCHANT); //3
		NPClite marie("Daggggggggggvid", WORKER); //4
		NPClite lary("Erigggggggggck", WORKER); //5
		NPClite luigi("Fraggggggggnk", WORKER); //6
		NPClite albert("Gaggggggggil", WORKER); //7
		NPClite dennis("Heggggggggnry", MAYOR); //8
		NPClite helen("Isagggggggggac", POLICE); //9
		NPClite merge("Jakggggggge", INNKEEPER); //10
		NPClite sigmund("Chgggggggggarles", PRIEST); //11
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
	  npcToNPClite(town,npcs);
	  simulation(town, seedint);
    // Removing the goodMurder stuff for now, since it's causing weird issues
    //if(goodMurder(town))
      goodKill = 1;
	}

  return seed;
}

// Has to return the seed because we'll need it later
std::string init(NPC *npcs, SDL_Renderer *renderer, WorldObject *worldObjects, std::string seed, NPClite *town){

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
  // Runs the simulation. I know, it looks so innocuous. Kinda nifty, right?
  seed = simTown(town, npcs, seed);
  std::cout << "Your seed is: " << seed << std::endl;

  // Weapons
  worldObjects[0].initObject("Art/Merder Objects/bat.png", renderer, 960, 855, 100, 50, 5, 5, 100, 50);
  worldObjects[1].initObject("Art/Merder Objects/Hammer_1.png", renderer, 1371, 1660, 50, 80, 5, 5, 50, 80);
  worldObjects[2].initObject("Art/Merder Objects/Pickaxe_1.png", renderer, 2158, 1760, 80, 80, 5, 5, 80, 80);
  worldObjects[3].initObject("Art/Merder Objects/butcher_knife.png", renderer, 2510, 855, 80, 80, 0, 0, 70, 70);

  // Make one of them a ghost
  int i;
  for(i = 0; i < 12; i++){
    if(town[i].isDead){
      npcs[i].ghostThisNPC();
    }
  }

  return seed;
}

void gameloop(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, bool farnan, std::string seed, int player_x, int player_y, int itemList[4]){
    // Initialize world texture, player texture, and camera
    SDL_Texture *bg = loadFiles(mapImgPath, renderer);
    SDL_Texture *interactPromptingTex = loadFiles(interactImgPath, renderer);
    Player *player = new Player(playerImgPath, renderer, player_x, player_y);
    SDL_Rect cam = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    NPClite town[12];

    // Create the NPCs (offloaded for brevity)
    NPC npcs[NPC_NUM];
    WorldObject worldObjects[WORLD_OBJECT_NUM];
    seed = init(npcs, renderer, worldObjects, seed, town);

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

    //Sign collisions
    //SDL_Rect sign1;
    SDL_Texture* townHallMessage;
    SDL_Texture* murderMessage;
    SDL_Texture* innMessage;
    SDL_Texture* churchMessage;
    SDL_Texture* factoryMessage;
    SDL_Texture* marketCentralMessage;
    SDL_Rect messageDestination;

    townHallMessage = loadFiles("Art/Messages/townHall.png", renderer);
    murderMessage = loadFiles("Art/Messages/murder.png", renderer);
    innMessage = loadFiles("Art/Messages/inn.png", renderer);
    churchMessage = loadFiles("Art/Messages/church.png", renderer);
    factoryMessage = loadFiles("Art/Messages/factory.png", renderer);
    marketCentralMessage = loadFiles("Art/Messages/marketCentral.png", renderer);


    // Collision dectction variables
    SDL_Rect collide;
    SDL_Rect convCollide;
    int hasCollided = 0;
    int hasSolved = 0;
    //Enter Game Loop
    while(!(*quit)) {
        //SDL time and delta value
        last_time = curr_time;
        curr_time = SDL_GetTicks();
        time_change = (curr_time - last_time) / 500.0f;
        frames_rendered++;
        fr_timer += (curr_time - last_time);
        if(fr_timer >= 1000){
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
        /*if (keyState[SDL_SCANCODE_C])
            enter_chat(e, &(*quit), keyState, renderer);*/

        // Open the inventory
        if (keyState[SDL_SCANCODE_I])
            open_inventory(e, &(*quit), keyState, renderer, itemList);

        //Talk to an NPC
        if (keyState[SDL_SCANCODE_X] && discussbool)
          dialogue(town, e, &(*quit), keyState, renderer, &(npcs[npcdiscuss]));

        // Quit may have changed during the dialogue, so it's best to check
        if (*quit) {
          if (hasSolved == 0) {
            std::fstream save;
            std::cout << seed << "\n";
            save.open("save.txt", std::fstream::out);
            save << seed << "\n" << player->positionPNG.x << "\n" << player->positionPNG.y << "\n";
            save << itemList[0] << "\n" << itemList[1] << "\n" << itemList[2] << "\n" << itemList[3] << "\n";
            save.close();
          } else {
            remove("save.txt");
          }
            return;
        }

        //Move Player
        //player->move(time_change, keyState, farnan);
        player->move(time_change, keyState, farnan, npcs, cBuilding, bBuilding, gBuilding, yBuilding, rBuilding);

        //Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);

        // Renders the background
        renderTexture(renderer, bg, cam, 0, 0, cam.w, cam.h, true);

        //render npcs
        int i=0;
        int indiscusscollider = 0;
        SDL_Rect positionPNG;

        //Signs
        SDL_Rect sign1;
        SDL_Rect sign1p2;
        SDL_Rect sign2;
        SDL_Rect sign3;
        SDL_Rect sign4;
        SDL_Rect sign5;
        SDL_Rect sign5p2;
        SDL_Rect sign6;

        // SDL_Texture* townHallMessage;
        // SDL_Texture* murderMessage;
        // SDL_Texture* innMessage;
        // SDL_Texture* churchMessage;
        // SDL_Texture* factoryMessage;
        // SDL_Texture* marketCentralMessage;
        // SDL_Rect messageDestination;

        // townHallMessage = loadFiles("Art/Messages/townHall.png", renderer);
        // murderMessage = loadFiles("Art/Messages/murder.png", renderer);
        // innMessage = loadFiles("Art/Messages/inn.png", renderer);
        // churchMessage = loadFiles("Art/Messages/church.png", renderer);
        // factoryMessage = loadFiles("Art/Messages/factory.png", renderer);
        // marketCentralMessage = loadFiles("Art/Messages/marketCentral.png", renderer);

        //Sign rect positions and widths
        sign1.x = 1000;
        sign1.y = 500;
        sign1.w = 60;
        sign1.h = 400;
        sign1p2.x = 1000;
        sign1p2.y = 500;
        sign1p2.w = 400;
        sign1p2.h = 300;

        sign2.x = 1525;
        sign2.y = 20;
        sign2.w = 800;
        sign2.h = 500;

        sign3.x = 950;
        sign3.y = 1250;
        sign3.w = 550;
        sign3.h = 500;

        sign4.x = 2175;
        sign4.y = 1550;
        sign4.w = 750;
        sign4.h = 405;

        sign5.x = 2450;
        sign5.y = 800;
        sign5.w = 275;
        sign5.h = 100;
        sign5p2.x = 2700;
        sign5p2.y = 450;
        sign5p2.w = 200;
        sign5p2.h = 725;

        sign6.x = 1600;
        sign6.y = 700;
        sign6.w = 750;
        sign6.h = 600;

        positionPNG.w = 30;
        positionPNG.h = 30;

        // check for collisions
        // Loop through twice to accomidate corner sections where the player may collide with 2 objects
        for (int axisI = 0; axisI < 2; axisI++){
          // Check NPC Collisions
          // for(i = 0; i < NPC_NUM; i++){
          //     if (npcs[i].NPCCollider.checkCollision(&(player->positionPNG), &collide)){
          //         hasCollided = 2;
          //     }
          // }

          //Locate player and show appropriate sign
          if(SDL_HasIntersection(&(player->positionPNG), &sign1) || SDL_HasIntersection(&(player->positionPNG), &sign1p2))
          {

            messageDestination.x = 0;
            messageDestination.y = 600;
            messageDestination.w = 300;
            messageDestination.h = 100;
            SDL_RenderCopy(renderer, townHallMessage, NULL, &messageDestination);
          }

          if(SDL_HasIntersection(&(player->positionPNG), &sign2))
          {

                messageDestination.x = 0;
                messageDestination.y = 600;
                messageDestination.w = 300;
                messageDestination.h = 100;
                SDL_RenderCopy(renderer, murderMessage, NULL, &messageDestination);
          }

          if(SDL_HasIntersection(&(player->positionPNG), &sign3))
          {

                messageDestination.x = 0;
                messageDestination.y = 600;
                messageDestination.w = 300;
                messageDestination.h = 100;
                SDL_RenderCopy(renderer, innMessage, NULL, &messageDestination);
          }

          if(SDL_HasIntersection(&(player->positionPNG), &sign4))
          {

                messageDestination.x = 0;
                messageDestination.y = 600;
                messageDestination.w = 300;
                messageDestination.h = 100;
                SDL_RenderCopy(renderer, churchMessage, NULL, &messageDestination);
          }

          if(SDL_HasIntersection(&(player->positionPNG), &sign5) || SDL_HasIntersection(&(player->positionPNG), &sign5p2))
          {

                messageDestination.x = 0;
                messageDestination.y = 600;
                messageDestination.w = 300;
                messageDestination.h = 100;
                SDL_RenderCopy(renderer, factoryMessage, NULL, &messageDestination);
          }

          if(SDL_HasIntersection(&(player->positionPNG), &sign6))
          {

                messageDestination.x = 0;
                messageDestination.y = 600;
                messageDestination.w = 300;
                messageDestination.h = 100;
                SDL_RenderCopy(renderer, marketCentralMessage, NULL, &messageDestination);
          }

          //Check building collisions
          // if (cBuilding.checkCollision(&(player->positionPNG), &collide)) {
          //     hasCollided = 3;
          // }
          // if (bBuilding.checkCollision(&(player->positionPNG), &collide)) {
          //     hasCollided = 3;
          // }
          // if (gBuilding.checkCollision(&(player->positionPNG), &collide)) {
          //     hasCollided = 3;
          // }
          // if (yBuilding.checkCollision(&(player->positionPNG), &collide)) {
          //     hasCollided = 3;
          // }
          // if (rBuilding.checkCollision(&(player->positionPNG), &collide)) {
          //     hasCollided = 3;
          // }
          // if(hasCollided){
          //   player->alterPosition(&collide);
          // } else {
          //   axisI = 2;
          // }
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

        //check for collision with signs

        if (itemList[0] == 0) {
            worldObjects[0].renderToScreen(renderer, cam);
        }
        if (itemList[1] == 0) {
            worldObjects[1].renderToScreen(renderer, cam);
        }
        if (itemList[2] == 0) {
            worldObjects[2].renderToScreen(renderer, cam);
        }
        if (itemList[3] == 0) {
            worldObjects[3].renderToScreen(renderer, cam);
        }

        //Check if we need to pick up some items
        if (worldObjects[0].checkCollision((&player->positionPNG), &collide)) {
            itemList[0] = 1;
        }
        if (worldObjects[1].checkCollision((&player->positionPNG), &collide)) {
            itemList[1] = 1;
        }
        if (worldObjects[2].checkCollision((&player->positionPNG), &collide)) {
            itemList[2] = 1;
        }
        if (worldObjects[3].checkCollision((&player->positionPNG), &collide)) {
            itemList[3] = 1;
        }

        // Only can discuss if we're within range
        discussbool = indiscusscollider;

        setCameraPosition(&cam, player->positionPNG);

        // Render Player
        player->render(renderer, &cam);

        // Check Collisions
        //player->collision(renderer, keyState);

        //Update Screen
        SDL_RenderPresent(renderer);
    }
}

#endif
