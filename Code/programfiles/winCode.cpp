#include "winCode.hpp"
#include "main_helper.hpp"

SDL_Rect useless_other2;

std::string winScreenPath = "Art/EndScreens/WinScreen.png";
std::string loseScreenPath = "Art/EndScreens/FailScreen.png";

void runWinScreen(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  SDL_Texture* winScreenTex = loadFiles(winScreenPath, renderer);

  //Clear screen
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

  bool inDiscussion = true;

  //Wait for user to exit chat
  while (SDL_PollEvent(&e) != 0 || inDiscussion)
  {
    //Quit application
    if(e.type == SDL_QUIT){
        *quit = true;
        return;
    }

    // Input Handler
    // Get the Keyboard State
    keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_Q]){
      *quit = true;
      return;
    }

    SDL_RenderClear(renderer);
    renderTexture(renderer, winScreenTex, useless_other2, 0, 0, 1280, 720, false);

    //Update Screen
    SDL_RenderPresent(renderer);
  }
}

// Yes, this is a straight up copy paste. Chill, bruh, I've got 10 hours till this project is due.
void runLoseScreen(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  SDL_Texture* winScreenTex = loadFiles(loseScreenPath, renderer);

  //Clear screen
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

  bool inDiscussion = true;

  //Wait for user to exit chat
  while (SDL_PollEvent(&e) != 0 || inDiscussion)
  {
    //Quit application
    if(e.type == SDL_QUIT){
        *quit = true;
        return;
    }

    // Input Handler
    // Get the Keyboard State
    keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_Q]){
      *quit = true;
      return;
    }

    SDL_RenderClear(renderer);
    renderTexture(renderer, winScreenTex, useless_other2, 0, 0, 1280, 720, false);

    //Update Screen
    SDL_RenderPresent(renderer);
  }
}
