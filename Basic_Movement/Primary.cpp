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
#include "../NPC_Gen/npc.hpp"
//#include "../NPC_Gen/npc.cpp"
#include "../NPC_Gen/bmp_edit.hpp"
//#include "../NPC_Gen/bmp_edit.cpp"

//Function Declarations
bool initialize();
SDL_Texture* loadFiles(std::string name);
void setCamera();
void clean();
//int initSprite(SDL_Renderer *renderer, int sizeX, int sizeY, int posX, int posY);
//void renderToScreen(SDL_Renderer *renderer, float timechange, float posX, float posY);

//Declaring Global Variables
const int MAP_WIDTH = 3840;
const int MAP_HEIGHT = 2160;
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int PLAYER_WIDTH = 64;
const int PLAYER_HEIGHT = 88;
const int X_FRAMES = 3;
const int Y_FRAMES = 4;

// Key state pointer for user input
const Uint8 *keyIn = SDL_GetKeyboardState(NULL);

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

SDL_Rect objRect;
SDL_Rect objPosition;

SDL_Texture* playerTexture;
SDL_Texture* bg;

SDL_Rect cropPNG;
SDL_Rect positionPNG;

SDL_Rect cam;
SDL_Scancode keys[4];
SDL_Rect tempRect;

SDL_Rect positionCastle;

SDL_Texture* interact;
SDL_Rect messageDestination;

SDL_Texture* churchMessage;
SDL_Rect churchDestination;

SDL_Texture* exitMessage;

float time_change = 0.0f;
bool isRunning;
float playerSpeed;
int frameWidth;
int frameHeight;
float counter;
int textureWidth;
int keyed = 0;

class Player {
    public:
        Player();
        void move(float change, const Uint8 *keyState);
        void render();
        bool collision();
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
    interact = loadFiles("Art/Messages/interact.png");
    churchMessage = loadFiles("Art/Messages/churchMessage.png");
    exitMessage = loadFiles("Art/Messages/exitMessage.png");
    
    //Chat Coming Soon
    SDL_Surface *chatSoon = IMG_Load("Art/chat_soon.png");
    SDL_Texture *chatSoonObject = SDL_CreateTextureFromSurface(renderer, chatSoon);

    Player player;

    //NPC Generation (not ready yet)
    //NPC blm("Benedict", "Art/NPCs/Blacksmithm.bmp", 0xFF000000, 0xFF000000, 0xFF888888);
    //blm.initSprite(renderer, 60, 88, 800, 250);
    //std::cout << blm.initSprite(renderer, 60, 88, 800, 250);

    srand(time(NULL));

    //Declare and Initialize Variables
    SDL_Event e;
    bool quit = false;
    bool next = false;
    int curr_time = 0;
    int last_time = 0;
    time_change = 0.0f;
    const Uint8 *keyPressed;

    cam = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

    //Logo Screen Loop
    while(!next && !quit) {

        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
               quit = true;
            }

            //If char 'C' is pressed (this will later become its on method/class)
            if (keyIn[SDL_SCANCODE_C]){
              std::cout << "Entering Chat\n";
              //inChat boolean value
              bool inChat = true;

              //Render chat coming soon page
              SDL_RenderCopy(renderer, chatSoonObject, NULL, NULL);
              SDL_RenderPresent(renderer);

              //Wait for user to exit chat
              while (SDL_PollEvent(&e) != 0 || (inChat == true && !quit))
              {
                //Quit application
                if(e.type == SDL_QUIT)
                    quit = true;

                if (keyIn[SDL_SCANCODE_E]){
                  inChat = false;
                  std::cout << "Exiting Chat\n";
                }
              }
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

        //Getting event
        while(SDL_PollEvent(&e) != 0)
        {
            //Quit application
            if(e.type == SDL_QUIT)
                quit = true;

            //If char 'C' is pressed (this will later become its on method/class)
            if (keyIn[SDL_SCANCODE_C]){
              std::cout << "Entering Chat\n";
              //inChat boolean value
              bool inChat = true;

              //Render chat coming soon page
              SDL_RenderCopy(renderer, chatSoonObject, NULL, NULL);
              SDL_RenderPresent(renderer);

              //Wait for user to exit chat
              while (SDL_PollEvent(&e) != 0 || (inChat == true && !quit))
              {
                //Quit application
                if(e.type == SDL_QUIT)
                    quit = true;

                if (keyIn[SDL_SCANCODE_E]){
                  inChat = false;
                  std::cout << "Exiting Chat\n";
                }
              }
            }
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

        //Render NPC (not ready yet)
        //blm.renderToScreen(renderer, time_change, 50, 60);

        player.collision();
        
        //Update Screen
        SDL_RenderPresent(renderer);
    }

    clean();

    return 0;
}

Player::Player() {

    //Query attributes of texture
    SDL_QueryTexture(playerTexture, NULL, NULL, &cropPNG.w, &cropPNG.h);
    SDL_QueryTexture(bg, NULL, NULL, &positionCastle.w, &positionCastle.h);

    //Speed of player
    playerSpeed = 175.0f;

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

    //Castle rect positions and widths
    positionCastle.x = 1915;
    positionCastle.y = 815;
    positionCastle.w = 38;
    positionCastle.h = 60;

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

bool Player::collision()
{
    if(positionPNG.x + positionPNG.w <= positionCastle.x || positionPNG.x >= positionCastle.x + positionCastle.w ||
        positionPNG.y + positionPNG.h <= positionCastle.y || positionPNG.y >= positionCastle.y + positionCastle.h)
    {
        keyed = 0;
        SDL_SetTextureColorMod(playerTexture, 255, 255, 255);
        return false;
    }

        SDL_SetTextureColorMod(playerTexture, 255, 0, 0);

        if (keyIn[SDL_SCANCODE_X] || keyed == 1){
            keyed = 1;
            //Display messages
            messageDestination.x = (SCREEN_WIDTH / 2) - 450;
            messageDestination.y = 600;
            messageDestination.w = 900;
            messageDestination.h = 100;
            SDL_RenderCopy(renderer, churchMessage, NULL, &messageDestination);
            messageDestination.x = 0;
            messageDestination.y = 0;
            messageDestination.w = 300;
            messageDestination.h = 100;
            SDL_RenderCopy(renderer, exitMessage, NULL, &messageDestination);
            SDL_SetTextureColorMod(playerTexture, 255, 0, 0);
        }
        
        if(keyIn[SDL_SCANCODE_Z] || keyed == 0) {
            keyed = 0;
            //Display interaction message and highlight character red when collision occurs
            messageDestination.x = 0;
            messageDestination.y = 0;
            messageDestination.w = 300;
            messageDestination.h = 100;
            SDL_RenderCopy(renderer, interact, NULL, &messageDestination);
        }


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
