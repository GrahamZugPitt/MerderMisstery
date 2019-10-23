#ifndef GLOBALVAR_COMMON
#define GLOBALVAR_COMMON
#include "common.hpp"
#endif


class Player {
	public:
		Player(SDL_Texture *playerTexture, SDL_Rect cropPNG);
		~Player();
		void render();
		void move(float change, const Uint8 *keyState);
		int xPosition = 0;
		int yPostition = 0;
};
