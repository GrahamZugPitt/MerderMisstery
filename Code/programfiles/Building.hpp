#ifndef BUILDING_H__
#define BUILDING_H__

#include <SDL2/SDL_image.h>
#include <string>
#include <vector>
#include <iostream>

#include "worldObjects.hpp"
constexpr int NORTH = 1;
constexpr int SOUTH = 2;
constexpr int EAST = 3;
constexpr int WEST = 4;

class Building: public WorldObject
{


public:

	SDL_Texture *texture;
	SDL_Rect mapPos;
	#ifdef OBJ_IMPLEMENTED
	std::vector worldObject interiorItems;
	#endif
	Building();
	Building(std::string spriteFile, SDL_Renderer *renderer, int mapX, int mapY, int mapW, int mapH,int colX, int colY, int colW, int colH);
	~Building();
	//void addWall(int posX, int posY, int width, int height);
	void addWalls(int width, SDL_Rect door, int facing);
};

#endif