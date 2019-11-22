#ifndef __PLAYER__
#define __PLAYER__
#include "common.hpp"


constexpr char UP = 0x01;
constexpr char DOWN = 0x02;
constexpr char LEFT = 0x04;
constexpr char RIGHT = 0x08;
class Player {
	public:
		Player(std::string playerTexturePath, SDL_Renderer *renderer);
		~Player();
		void render(SDL_Renderer *renderer, SDL_Rect *cam);
		void move(float change, const Uint8 *keyState, bool farnan);
		bool collision(SDL_Renderer *renderer, const Uint8 *keyState);
    void alterPosition(SDL_Rect *collider);

		// playerSpeed measured in px / s
		float playerSpeed = 250.f;
		int colorKeyed = 0;
		int textureWidth;
		bool isRunning;
		int frameWidth;
		int frameHeight;
		float counter;
		float xvel;
		float yvel;
    int direction = 0;
    int lastX = 0;
    int lastY = 0;
    int overWriteX = 0;
    int overWriteY = 0;
		SDL_Texture *playerTexture;
		SDL_Texture *interactTexture;
		SDL_Texture *churchMessageTexture;
		SDL_Texture *exitMessageTexture;
		SDL_Rect cropPNG;
    SDL_Rect positionPNG;
};

#endif
