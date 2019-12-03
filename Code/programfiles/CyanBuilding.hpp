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
	SDL_Rect rect = {911, 365, 557, 750};
	Chair chairs[6];
	Table tables[3];
	Collidable walls[8];
	//Collidable fences[7];
	//Sign sign;

};

#endif
