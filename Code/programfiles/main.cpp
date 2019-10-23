//
//  Primary.cpp
//  Game Design
//
//  Original File created by Darpun Kohli
//  Scrolling Added by Shawn Blake
//

#include "main_helper.hpp"
#include "gameloop.hpp"
#include "Player.hpp"

int main(int argc, char *argv[]) {
    if (!initialize()) {
        std::cout <<  "Couldn't Initialize. Cleaning and exiting now." << std::endl;
        clean();
        return 1;
    }
    
    SDL_Texture* logoScreen = loadFiles("Art/Logo/Logo.png");
    
    srand(time(NULL));
    
    //Declare and Initialize Variables
    SDL_Event e;
    bool quit = false;
    bool next = false;
    int curr_time = 0;
    int last_time = 0;
    float time_change = 0.0f;
    const Uint8 *keyPressed;
    
    cam = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    
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
    
    gameloop(e, quit, curr_time, last_time, time_change, keyPressed);

    clean();
    
    return 0;
}

