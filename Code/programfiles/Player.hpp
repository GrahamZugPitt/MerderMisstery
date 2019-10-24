#ifndef GLOBALVAR_COMMON
#define GLOBALVAR_COMMON
#include "common.hpp"
#endif


class Player {
	public:
		Player(std::string playerTexturePath, SDL_Rect *cropPNG, SDL_Rect *positionPNG, SDL_Renderer *renderer);
		~Player();
		void render(SDL_Rect *cropPNG, SDL_Rect *positionPNG, SDL_Renderer *renderer, SDL_Rect *cam);
		void move(float change, const Uint8 *keyState, SDL_Rect *cropPNG, SDL_Rect *positionPNG);
		int xPosition = 0;
		int yPostition = 0;
		float playerSpeed = 250.f;
		int textureWidth;
		bool isRunning;
		int frameWidth;
		int frameHeight;
		float counter;
		SDL_Texture *playerTexture;
};
