#include "inventory.hpp"
#include "main_helper.hpp"

SDL_Rect useless_other;

std::string inventoryScreenPath = "Art/Inventory/inventoryScreen.png";
std::string singlePlayerPath = "Art/Player/SinglePlayer.png";

void open_inventory(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  SDL_Texture* inventoryScreenTex = loadFiles(inventoryScreenPath, renderer);
  SDL_Texture* playerTex = loadFiles(singlePlayerPath, renderer);

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

    if (keyState[SDL_SCANCODE_Q])
        return;

    SDL_RenderClear(renderer);
    renderTexture(renderer, inventoryScreenTex, useless_other, 0, 0, 1280, 720, false);
    renderTexture(renderer, playerTex, useless_other, 800, 100, 168, 216, false);

    //Update Screen
    SDL_RenderPresent(renderer);
  }
}
