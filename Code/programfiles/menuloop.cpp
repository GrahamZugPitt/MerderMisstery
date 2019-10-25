#include "gameloop.hpp"
#include "main_helper.hpp"
#include "chat.hpp"

std::string logoImgPath = "Art/Logo/Logo.png";

void menuloop(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  // Load the title screens
  SDL_Texture* logoScreen = loadFiles(logoImgPath, renderer);
  bool next = false;

  //Logo Screen Loop
  while(!next && !(*quit)) {
      while(SDL_PollEvent(&e) != 0) {
          if(e.type == SDL_QUIT) {
            (*quit) = true;
          }
        }
        // Get the Keyboard State
        keyState = SDL_GetKeyboardState(NULL);

        // Start the game
        if (keyState[SDL_SCANCODE_SPACE])
          next = true;

        // Open Chat room
        if (keyState[SDL_SCANCODE_C])
            enter_chat(e, &(*quit), keyState, renderer);

        // Quit the game
        if (keyState[SDL_SCANCODE_Q])
            (*quit) = true;

        //Logo Screen Render
        SDL_RenderCopy(renderer, logoScreen, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
}
