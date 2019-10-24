//
//
//  Original File created by Darpun Kohli
//  Scrolling Added by Shawn Blake
//

#include "main_helper.hpp"
#include "gameloop.hpp"
#include "Player.hpp"

// Declaring variables that will be used later
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect objRect;
SDL_Rect objPosition;
SDL_Scancode keys[4];
std::string logoImgPath = "Art/Logo/Logo.png";

int main(int argc, char *argv[]) {
    if (!initialize(&window, &renderer)) {
        std::cout <<  "Couldn't Initialize. Cleaning and exiting now." << std::endl;
        clean(window, renderer);
        return 1;
    }

    SDL_Texture* logoScreen = loadFiles(logoImgPath, renderer);

    srand(time(NULL));

    //Declare and Initialize Variables
    SDL_Event e;
    bool quit = false;
    bool next = false;
    int curr_time = 0;
    int last_time = 0;
    float time_change = 0.0f;
    const Uint8 *keyPressed;

    keyPressed = SDL_GetKeyboardState(NULL);

    //Logo Screen Loop
    while(!next && !quit) {

        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
               quit = true;
            }
        }

        switch (e.button.button) {
            case SDL_BUTTON_LEFT:
                next = true;
                break;
        }

        //Logo Screen Render
        SDL_RenderCopy(renderer, logoScreen, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    gameloop(e, quit, curr_time, last_time, time_change, keyPressed, renderer);

    clean(window, renderer);

    return 0;
}
