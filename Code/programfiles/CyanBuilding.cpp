#include "CyanBuilding.hpp"

CyanBuilding::CyanBuilding(){
	walls[0].setRectValues(930, 408, 18, 552);
	walls[1].setRectValues(1066, 815, 18, 145);
	walls[2].setRectValues(1084, 815, 372, 18);
	walls[3].setRectValues(1438, 408, 18, 407);
	walls[4].setRectValues(948, 408, 490, 18);
	walls[5].setRectValues(1097, 578, 242, 55);
}

bool CyanBuilding::checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap){
	if(!SDL_HasIntersection(&rect, playerRect)){
		return false; 
	} else {
		
		for(int i = 0; i < 8; i++){
			if (walls[i].checkCollision(playerRect, overlap)){
				return true;
			}
		}
	}
	return false;
}

CyanBuilding::~CyanBuilding(){}
