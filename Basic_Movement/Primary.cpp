//
//  Primary.cpp
//  Game Design
//
//  Original File created by Darpun Kohli
//  Scrolling Added by Shawn Blake
//

//Include Statements
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

//Function Declarations
bool initialize();
SDL_Texture* loadFiles(std::string name);
void setCamera();
void clean();

//Declaring Global Variables
const int MAP_WIDTH = 3840;
const int MAP_HEIGHT = 2160;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int PLAYER_WIDTH = 64;
const int PLAYER_HEIGHT = 88;
const int X_FRAMES = 3;
const int Y_FRAMES = 4;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Rect objRect;
SDL_Rect objPosition;
SDL_Texture* playerTexture;
SDL_Rect cropPNG;
SDL_Rect positionPNG;
SDL_Rect cam;
SDL_Scancode keys[4];

bool isRunning;
float playerSpeed;
int frameWidth;
int frameHeight;
float counter;
int textureWidth;

class Player {
    public:
        Player();
        void move(float change, const Uint8 *keyState);
        void render();
        ~Player();
        int xPosition = 0;
        int yPosition = 0;
        int xVelocity = 0;
        int yVelocity = 0;
};

int main(int argc, char *argv[]) {
    if (!initialize()) {
        std::cout <<  "Couldn't Initialize. Cleaning and exiting now." << std::endl;
        clean();
        return 1;
    }
    
    SDL_Texture* bg = loadFiles("Art/Tiles/Map.png");
    SDL_Texture* logoScreen = loadFiles("Art/Logo/Logo.png");
    playerTexture = loadFiles("Art/Player/PlayerSpriteSheet.png");
    
    Player player;
    
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
    
    //Enter Game Loop
    while(!quit) {
        //SDL time and delta value
        last_time = curr_time;
        curr_time = SDL_GetTicks();
        time_change = (curr_time - last_time) / 500.0f;
        
        //Quit application
        while(SDL_PollEvent(&e) != 0)
        {
            //Getting event
            if(e.type == SDL_QUIT)
                quit = true;
        }
        
        //Get current state of keyboard
        keyPressed = SDL_GetKeyboardState(NULL);
        
        //Move Player
        player.move(time_change, keyPressed);
        
        setCamera();
        
        //Clear screen
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(renderer);
        
        //Grass image
        SDL_Rect tempRect;
        tempRect.x = 0;
        tempRect.y = 0;
        tempRect.w = cam.w;
        tempRect.h = cam.h;
        SDL_RenderCopy(renderer, bg, &cam, &tempRect);
        
        //Render Player
        player.render();
        
        //Update Screen
        SDL_RenderPresent(renderer);
    }
    
    clean();
    
    return 0;
}

Player::Player() {
    
    //Query attributes of texture
    SDL_QueryTexture(playerTexture, NULL, NULL, &cropPNG.w, &cropPNG.h);
    
    //Speed of player
    playerSpeed = 250.0f;
    
    //Position of player sprite
    positionPNG.x = (MAP_WIDTH / 2) - (PLAYER_WIDTH / 2);
    positionPNG.y = (MAP_HEIGHT / 2) - (PLAYER_HEIGHT / 2);
    
    //Set textureWidth to current crop width
    textureWidth = cropPNG.w;
    
    //Crop player sprite surface based on number of image types (in this case 12; 3x4)
    cropPNG.w = cropPNG.w / X_FRAMES;
    cropPNG.h = cropPNG.h / Y_FRAMES;
    
    //Constant width of our frame
    positionPNG.w = cropPNG.w;
    positionPNG.h = cropPNG.h;
    frameWidth = positionPNG.w;
    frameHeight = positionPNG.h;
    
    //Running flag
    isRunning = false;
}

void Player::move(float change, const Uint8 *keyState) {
    
    //Set isRunning flag to true
    isRunning = true;

    //Determine which directional keypad arrow is clicked and apply appropriate movement / image
    if(keyState[SDL_SCANCODE_UP]) {
        positionPNG.y -= playerSpeed * change;
        cropPNG.y = frameHeight * 3;
    }
    else if(keyState[SDL_SCANCODE_DOWN]) {
        positionPNG.y += playerSpeed * change;
        cropPNG.y = 0;
    }
    else if(keyState[SDL_SCANCODE_LEFT]) {
        positionPNG.x -= playerSpeed * change;
        cropPNG.y = frameHeight;
    }
    else if(keyState[SDL_SCANCODE_RIGHT]) {
        positionPNG.x += playerSpeed * change;
        cropPNG.y = frameHeight * 2;
    }
    //User is not clicking key, so player is no longer running
    else {
        isRunning = false;
    }
    //Animate the player sprite by flashing frames
    if(isRunning) {
        counter += change;
        if(counter >= 0.08f) {
            counter = 0;
            cropPNG.x += frameWidth;
            if(cropPNG.x >= textureWidth)
                cropPNG.x = 0;
        }
    }
    
    //Reset to original position since inactive
    else {
        counter = 0;
        cropPNG.x = frameWidth;
    }
    
    if (positionPNG.x < 0) {
        positionPNG.x += playerSpeed * change;
    }
    if (positionPNG.x + PLAYER_WIDTH > MAP_WIDTH) {
        positionPNG.x -= playerSpeed * change;
    }
    if (positionPNG.y < 0) {
        positionPNG.y += playerSpeed * change;
    }
    if (positionPNG.y + PLAYER_HEIGHT > MAP_HEIGHT) {
        positionPNG.y -= playerSpeed * change;
    }
    
}

void Player::render() {
    SDL_Rect tempRect;
    tempRect.x = positionPNG.x - cam.x;
    tempRect.y = positionPNG.y - cam.y;
    tempRect.w = cropPNG.w;
    tempRect.h = cropPNG.h;
    
    SDL_RenderCopy(renderer, playerTexture, &cropPNG, &tempRect);
}

Player::~Player() {
    SDL_DestroyTexture(playerTexture);
}

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
