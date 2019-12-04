#include "GreenBuilding.hpp"

GreenBuilding::GreenBuilding(){
    walls[0].setRectValues(2283, 1950, 677, 15);
    walls[1].setRectValues(2942, 1475, 17, 489);
    walls[2].setRectValues(2110, 1475, 849, 17);
    walls[3].setRectValues(2110, 1475, 17, 491);
    walls[4].setRectValues(2110, 1947, 69, 19);
}

bool GreenBuilding::checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap){
    if(!SDL_HasIntersection(&rect, playerRect)){
        return false;
    } else {
        for(auto wall: walls){
            if (wall.checkCollision(playerRect, overlap)){
                return true;
            }
        }
    }
    return false;
}

GreenBuilding::~GreenBuilding(){}

