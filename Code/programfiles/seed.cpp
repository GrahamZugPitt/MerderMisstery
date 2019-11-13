#include "seed.hpp"
#include "main_helper.hpp"

// The Seed page template
std::string seedPagePath = "Art/Logo/Seed_Page.png";

void enter_seed(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  std::cout << "Enter Seed\n";
  //inSeed boolean value
  bool inSeed = true;

  SDL_Texture* seedPageScreen = loadFiles(seedPagePath, renderer);

  //Render seed page background
  SDL_RenderCopy(renderer, seedPageScreen, NULL, NULL);
  SDL_RenderPresent(renderer);

  //Wait for user to exit chat
  while (SDL_PollEvent(&e) != 0 || (inSeed == true && !(*quit)))
  {
    //Quit application
    if(e.type == SDL_QUIT)
        *quit = true;

    // Get the Keyboard State
    keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_RETURN])
    {
      inSeed = false;
      // SDL_RenderClear(renderer);
      //make sure to end things and return seed
    }
  }
}
