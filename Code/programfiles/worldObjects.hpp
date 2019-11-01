#include <iostream>
#include <string>
#include "common.hpp"
#include "collision.hpp"

// No parameters as yet, that'll probably change
SDL_Rect getCastle();

#ifndef WORLD_OBJECT_H__
#define WORLD_OBJECT_H__

class WorldObject: public Collidable
{
public:

	SDL_Texture *texture;
	SDL_Rect mapPos;
	SDL_Rect screenPos;
	SDL_Rect collisionBox;
	SDL_Rect crop;
	WorldObject();
	WorldObject(std::string spriteFile, SDL_Renderer *renderer, int mapX, int mapY, int mapW, int mapH,int colX, int colY, int colW, int colH);
	~WorldObject();

	int initObject(std::string spriteFile, SDL_Renderer *renderer, int mapX, int mapY, int mapW, int mapH,int colX, int colY, int colW, int colH);
	void renderToScreen(SDL_Renderer *renderer, SDL_Rect camPos);
};
#endif
