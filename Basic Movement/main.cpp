//
//  main.cpp
//  Game_Design
//
//  Created by Darpun Kohli on 9/26/19.
//

//Includes
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include"Player.hpp"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Player image dimensions
const int PLAYER_WIDTH = 64;
const int PLAYER_HEIGHT = 88;

//Smith dimensions
const int SMITH_WIDTH = 60;
const int SMITH_HEIGHT = 88;


//Window pointer
SDL_Window *window = NULL;

//Renderer
SDL_Renderer *renderer = NULL;

//SDL_Rects for objects
SDL_Rect objRect;
SDL_Rect objPosition;

//Free memory and close subsystems
void close();

//Free memory and close subsystems
void close()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    window = NULL;
    renderer = NULL;
    
    IMG_Quit();
    SDL_Quit();
}

//Main method
int main(int argc, char *argv[])
{
    //Declare and initialize variables
    int curr_time = 0;
    int last_time = 0;
    float time_change = 0.0f;
    const Uint8 *keyPressed;
    
    //Player set to center of screen
    int initial_x = (SCREEN_WIDTH / 2) - (PLAYER_WIDTH / 2);
    int initial_y = (SCREEN_HEIGHT / 2) - (PLAYER_HEIGHT / 2);
    
    //Start SDL video subsystem
    SDL_Init(SDL_INIT_VIDEO);
    
    //Create window and center on page at 1280x720
    window = SDL_CreateWindow("Merder Misstery", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    
    //Initialize renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    //Logo background
    SDL_Surface *logo = IMG_Load("Art/Logo/Logo.png");
    SDL_Texture *logoScreen = SDL_CreateTextureFromSurface(renderer, logo);
    
    //Grass background
    SDL_Surface *grass = IMG_Load("Art/Tiles/TemporaryGrass.png");
    SDL_Texture *grassBackground = SDL_CreateTextureFromSurface(renderer, grass);
    
    //Horse
    SDL_Surface *horse = IMG_Load("Art/Decor/Horse.png");
    SDL_Texture *horseObject = SDL_CreateTextureFromSurface(renderer, horse);
    
    //Well
    SDL_Surface *well = IMG_Load("Art/Decor/WaterWell.png");
    SDL_Texture *wellObject = SDL_CreateTextureFromSurface(renderer, well);
    
    //Set player configuration and send to Player class
    Player player1(renderer, "Art/Player/PlayerSpriteSheet.png", initial_x, initial_y);
    
    // Blacksmith
    SDL_Rect smithCrop;
    smithCrop.x = 7;
    smithCrop.y = 0;//SMITH_HEIGHT;
    smithCrop.w = SMITH_WIDTH;
    smithCrop.h = SMITH_HEIGHT;
    float smithcounter =0;
    SDL_Surface *blacksmith = IMG_Load("Art/NPCs/Blacksmithm.bmp");
    SDL_Texture *smithObject = SDL_CreateTextureFromSurface(renderer, blacksmith);
    //Quit and next flags
    bool quit = false;
    bool next = false;
    
    //SDL event
    SDL_Event e;
    
    //Logo screen loop
    while(!next && !quit) {
        
        //Quit application
        while(SDL_PollEvent(&e) != 0)
        {
            //Getting event
            if(e.type == SDL_QUIT)
                quit = true;
        }
        
        switch (e.button.button)
        {
            case SDL_BUTTON_LEFT:
                //Load surfaceGrass PNG and blit on surfaceWindow
                next = true;
                break;
        }
        
        //Logo screen render
        SDL_RenderCopy(renderer, logoScreen, NULL, NULL);
        SDL_RenderPresent(renderer);
    }
    
    
    //Enter game loop
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
        
        //Update player movement
        player1.Update(time_change, keyPressed);
        
        //Rendering
        SDL_RenderClear(renderer);
        
        //Static grass image
        SDL_RenderCopy(renderer, grassBackground, NULL, NULL);
        
        /***** Object rendering DEMO ******/
        /*TODO: Collision with objects as well as ensuring player appears behind object if player's x position is less than that
         of the object*/
        
         objRect.w = 160;
         objRect.h = 96;
         objPosition.w = objRect.w;
         objPosition.h = objRect.h;
         objPosition.x = 200;
         objPosition.y = 200;
        
        //Horse
        SDL_RenderCopy(renderer, horseObject, &objRect, &objPosition);
        
        objPosition.x += 250;
        objPosition.y += 100;
        
        //Well
        SDL_RenderCopy(renderer, wellObject, &objRect, &objPosition);
        
        // Animated Blacksmith
        objPosition.x += 300;
        objPosition.y -= 100;

        objPosition.w = SMITH_WIDTH;
        objPosition.h = SMITH_HEIGHT;
        smithcounter += time_change;
        if(smithcounter >= 0.3f)
        {
            smithcounter = 0;
            smithCrop.x += SMITH_WIDTH;
            if(smithCrop.x >= 3*SMITH_WIDTH)
                smithCrop.x = 7;
        }
        SDL_RenderCopy(renderer, smithObject, &smithCrop, &objPosition);
        /***** Object rendering DEMO ******/
        
        //Render player
        player1.RenderOnScreen(renderer);
        
        SDL_RenderPresent(renderer);
    }
    
    //Free memory and close subsystems
    close();
    
    return 0;
}
