#include "inventory.hpp"
#include "main_helper.hpp"

SDL_Rect useless_other;

std::string inventoryScreenPath = "Art/Inventory/inventoryScreen.png";
std::string singlePlayerPath = "Art/Player/SinglePlayer.png";
std::string weaponOnePath = "Art/Merder Objects/bat.png";
std::string weaponTwoPath = "Art/Merder Objects/Hammer_1.png";
std::string weaponThreePath = "Art/Merder Objects/Pickaxe_1.png";
std::string weaponFourPath = "Art/Merder Objects/butcher_knife.png";

void open_inventory(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, int itemList[]){
    SDL_Texture* inventoryScreenTex = loadFiles(inventoryScreenPath, renderer);
    SDL_Texture* playerTex = loadFiles(singlePlayerPath, renderer);
    SDL_Texture* weaponOneTex = loadFiles(weaponOnePath, renderer);
    SDL_Texture* weaponTwoTex = loadFiles(weaponTwoPath, renderer);
    SDL_Texture* weaponThreeTex = loadFiles(weaponThreePath, renderer);
    SDL_Texture* weaponFourTex = loadFiles(weaponFourPath, renderer);

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
    if (itemList[0] == 1) {
        renderTexture(renderer, weaponOneTex, useless_other, 180, 500, 128, 64, false);
    }
    if (itemList[1] == 1) {
        renderTexture(renderer, weaponTwoTex, useless_other, 480, 500, 64, 96, false);
    }
    if (itemList[2] == 1) {
        renderTexture(renderer, weaponThreeTex, useless_other, 720, 500, 96, 96, false);
    }
    if (itemList[3] == 1) {
        renderTexture(renderer, weaponFourTex, useless_other, 980, 500, 96, 96, false);
    }
        
    //Update Screen
    SDL_RenderPresent(renderer);
    }
}
