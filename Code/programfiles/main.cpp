//
//
//  Main File for the MerderMisstery Game
//  Team Alpha Game Collective
//
#include "main_helper.hpp"
#include "gameloop.hpp"
#include "Player.hpp"
#include "menuloop.hpp"

#include <fstream>

// Need a window and a renderer, for obvious purposes
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
bool farnan = false;

int main(int argc, char *argv[]) {
    if(argc == 2){
      farnan = true;
    }

    if (!initialize(&window, &renderer)) {
        std::cout <<  "Couldn't Initialize. Cleaning and exiting now." << std::endl;
        clean(window, renderer);
        return 1;
    }

    //Declare and Initialize Variables
    SDL_Event e;
    bool quit = false;
    const Uint8 *keyState;
    if (TTF_Init() < 0) printf("Failed to initialize TTF!\n");

    // Set the keystate just to suppress an error
    keyState = SDL_GetKeyboardState(NULL);

    while(!quit){
      int initial_x = 2275;
      int initial_y = 136;
      int itemList[] = {0, 0, 0, 0};
      menuloop(e, &quit, keyState, renderer);

      std::fstream save;
      save.open("save.txt", std::fstream::in);
      if (save) {
        std::string in_str = "";
        std::getline(save, in_str);  // First read is for seed
        std::getline(save, in_str);  // Second read is for location.x
        initial_x = std::stoi(in_str);
        std::getline(save, in_str);  // Third read is for location.y
        initial_y = std::stoi(in_str);
        std::getline(save, in_str);  // Third read is for item 1
        itemList[0] = std::stoi(in_str);
        std::getline(save, in_str);  // Third read is for item 2
        itemList[1] = std::stoi(in_str);
        std::getline(save, in_str);  // Third read is for item 3
        itemList[2] = std::stoi(in_str);
        std::getline(save, in_str);  // Third read is for item 4
        itemList[3] = std::stoi(in_str);
      }
      save.close();


      if(!quit){
        // During the game, quit will have been set to true
        // Please don't ask why I'm handling this so poorly
        // You weren't there
        // You wouldn't understand
        gameloop(e, &quit, keyState, renderer, farnan, seed, initial_x, initial_y, itemList);
      }
    }


    clean(window, renderer);

    return 0;
}
