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
		bool collision(SDL_Renderer *renderer, const Uint8 *keyState);

		// playerSpeed measured in px / s
		float playerSpeed = 175.f;
		int colorKeyed = 0;
		int textureWidth;
		bool isRunning;
		int frameWidth;
		int frameHeight;
		float counter;
		SDL_Texture *playerTexture;
		SDL_Texture *interactTexture;
		SDL_Texture *churchMessageTexture;
		SDL_Texture *exitMessageTexture;
		SDL_Rect cropPNG;
    SDL_Rect positionPNG;
};
