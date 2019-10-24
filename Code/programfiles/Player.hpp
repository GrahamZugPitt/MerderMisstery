#ifndef GLOBALVAR_COMMON
#define GLOBALVAR_COMMON
#include "common.hpp"
#endif


class Player {
	public:
		Player(std::string playerTexturePath, SDL_Renderer *renderer);
		~Player();
		void render(SDL_Renderer *renderer, SDL_Rect *cam);
		void move(float change, const Uint8 *keyState);
		int xPosition = 0;
		int yPostition = 0;
		// playerSpeed measured in px / s
		float playerSpeed = 250.f;

		int textureWidth;
		bool isRunning;
		int frameWidth;
		int frameHeight;
		float counter;
		SDL_Texture *playerTexture;
		SDL_Rect cropPNG;
    SDL_Rect positionPNG;
};
