//
//  Player.cpp
//  Game_Design
//
//  Created by Darpun Kohli on 10/27/19.
//

//Includes
#include "Player.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>

//Sprite frames
int X_FRAMES = 3;
int Y_FRAMES = 4;

//Player class: Player method
Player::Player(SDL_Renderer *renderer, std::string filePath, int x, int y)
{
    //Create surface from PlayerSpriteSheet.png
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    
    //Check if we can pull PlayerSpriteSheet.png
    if (surface == NULL)
        printf("Error loading Player Sprite Sheet!");
    else
    {
        //Create texture from player sprite surface we just created
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        
        //Check if texture was able to be made
        if (texture == NULL)
            printf("Error creating texture from player sprite sheet surface!");
    }
    
    //Free memory of player sprite surface
    SDL_FreeSurface(surface);
    
    //Query attributes of texture
    SDL_QueryTexture(texture, NULL, NULL, &cropPNG.w, &cropPNG.h);
    
    //Speed of player
    playerSpeed = 150.0f;
    
    //Position of player sprite on screen passed in from main method
    positionPNG.x = x;
    positionPNG.y = y;
    
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

//Player class: Update class
void Player::Update(float change, const Uint8 *keyState)
{
    //Set isRunning flag to true
    isRunning = true;
    
    //Determine which directional keypad arrow is clicked and apply appropriate movement / image
    if(keyState[SDL_SCANCODE_UP])
    {
        positionPNG.y -= playerSpeed * change;
        cropPNG.y = frameHeight * 3;
    }
    else if(keyState[SDL_SCANCODE_DOWN])
    {
        positionPNG.y += playerSpeed * change;
        cropPNG.y = 0;
    }
    else if(keyState[SDL_SCANCODE_LEFT])
    {
        positionPNG.x -= playerSpeed * change;
        cropPNG.y = frameHeight;
    }
    else if(keyState[SDL_SCANCODE_RIGHT])
    {
        positionPNG.x += playerSpeed * change;
        cropPNG.y = frameHeight * 2;
    }
    
    //User is not clicking key, so player is no longer running
    else {
        isRunning = false;
    }
    
    //Animate the player sprite by flashing frames
    if(isRunning)
    {
        counter += change;
        
        if(counter >= 0.08f)
        {
            counter = 0;
            cropPNG.x += frameWidth;
            if(cropPNG.x >= textureWidth)
                cropPNG.x = 0;
        }
    }
    
    //Reset to original position since inactive
    else
    {
        counter = 0;
        cropPNG.x = frameWidth;
    }
}

//Render player
void Player::RenderOnScreen(SDL_Renderer *renderer)
{
    SDL_RenderCopy(renderer, texture, &cropPNG, &positionPNG);
}

//Player method destructor
Player::~Player()
{
    SDL_DestroyTexture(texture);
}
