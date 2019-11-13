//
//
//  Main File for the MerderMisstery Game
//  Team Alpha Game Collective
//

#include "main_helper.hpp"
#include "gameloop.hpp"
#include "Player.hpp"
#include "menuloop.hpp"

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

    // Set the keystate just to suppress an error
    keyState = SDL_GetKeyboardState(NULL);

    menuloop(e, &quit, keyState, renderer);

    gameloop(e, &quit, keyState, renderer, farnan);

    clean(window, renderer);

    return 0;
}
