#ifndef _CYANBUILDING_HPP__
#define _CYANBUILDING_HPP__

#include "objects.hpp"
#include "common.hpp"

class CyanBuilding{
public:
	CyanBuilding();
	~CyanBuilding();
	bool checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap);


private:
	SDL_Rect rect = {887, 593, 562, 608};
	Chair chairs[4];
	Table tables[2];
	Collidable walls[7];
	//Collidable fences[7];
	//Sign sign;

};

#endif