#include "CyanBuilding.hpp"

CyanBuilding::CyanBuilding(){
	std::cout << "setting chairs" << std::endl;
	chairs[0].setPos(992, 712);
	chairs[1].setPos(1114, 712);
	chairs[2].setPos(1222, 712);
	chairs[3].setPos(1344, 712);

	std::cout << "setting tables" << std::endl;
	tables[0].setPos(1031, 696);
	tables[1].setPos(1261, 699);

	walls[0].setRectValues(1059, 828, 356,10);
	walls[1].setRectValues(1415, 636, 10, 196);
	walls[2].setRectValues(955, 631, 464, 10);
	walls[3].setRectValues(945, 640, 10, 318);
	walls[4].setRectValues(945, 955, 11, 10);
	walls[5].setRectValues(1045, 955, 10, 10);
	walls[6].setRectValues(1055, 828, 10, 130);
}

bool CyanBuilding::checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap){
	if(!SDL_HasIntersection(&rect, playerRect)){
		return false; 
	} else {
		for(auto chair: chairs){
			if (chair.checkCollision(playerRect, overlap)){
				return true;
			}
		}
		for(auto table: tables){
			if (table.checkCollision(playerRect, overlap)){
				return true;
			}
		}
		for(auto wall: walls){
			if (wall.checkCollision(playerRect, overlap)){
				std::cout << "wall collision" << overlap->x << " " << overlap->y << " " << overlap->w << " " << overlap->h << std::endl;
				return true;
			}
		}
	}
	return false;
}

CyanBuilding::~CyanBuilding(){}