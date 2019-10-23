//
// Setup to define everything for our main file
// Game Design
//
// Team Alpha Game Collective
//

//Include Statements
#include "main_helper.hpp"

//Functions as declared
bool initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
        std::cout << "Warning: Linear texture filtering not enabled!" << std::endl;
    }

    window = SDL_CreateWindow("Merder Misstery", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return  false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return  false;
    }

    int imgFlags = IMG_INIT_PNG;
    int retFlags = IMG_Init(imgFlags);
    if(retFlags != imgFlags) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    // Set renderer draw/clear color
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

    return true;
}

SDL_Texture* loadFiles(std::string name) {
    SDL_Texture* newText = nullptr;

    SDL_Surface* startSurf = IMG_Load(name.c_str());
    if (startSurf == nullptr) {
        std::cout << "Unable to load image " << name << "! SDL Error: " << SDL_GetError() << std::endl;
        return nullptr;
    }

    newText = SDL_CreateTextureFromSurface(renderer, startSurf);
    if (newText == nullptr) {
        std::cout << "Unable to create texture from " << name << "! SDL Error: " << SDL_GetError() << std::endl;
    }

    SDL_FreeSurface(startSurf);

    return newText;
}

void setCamera() {
    cam.x = (positionPNG.x + PLAYER_WIDTH / 2) - SCREEN_WIDTH / 2;
    cam.y = (positionPNG.y + PLAYER_HEIGHT / 2) - SCREEN_HEIGHT / 2;

    if (cam.x < 0) {
        cam.x = 0;
    }
    if (cam.y < 0) {
        cam.y = 0;
    }
    if (cam.x > MAP_WIDTH - cam.w) {
        cam.x = MAP_WIDTH - cam.w;
    }
    if (cam.y > MAP_HEIGHT - cam.h) {
        cam.y = MAP_HEIGHT - cam.h;
    }
}

void clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);

    window = NULL;
    renderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

