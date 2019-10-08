//
//  Player.hpp
//  Game_Design
//
//  Created by Darpun Kohli on 10/27/19.
//

#ifndef Player_hpp
#define Player_hpp

#include <SDL2/SDL.h>
#include <string>
#include <stdio.h>

class Player
{
    private:
        SDL_Rect cropPNG;
        SDL_Texture *texture;
        SDL_Scancode keys[4];
        float playerSpeed;
        float counter;
        int frameWidth, frameHeight;
        int textureWidth;
        bool isRunning;
    public:
        SDL_Rect positionPNG;
        Player(SDL_Renderer *renderer, std::string filePath, int x, int y);
        ~Player();
        void Update(float delta, const Uint8 *keyState);
        void RenderOnScreen(SDL_Renderer *renderer);
};

#endif /* Player_hpp */
