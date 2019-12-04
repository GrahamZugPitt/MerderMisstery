#include "GreenBuilding.hpp"

GreenBuilding::GreenBuilding(){
    walls[0].setRectValues(2109, 1474, 17, 492);
    walls[1].setRectValues(2126, 1474, 818, 17);
    walls[2].setRectValues(2944, 1474, 16, 492);
    walls[3].setRectValues(2267, 1949, 677, 17);
    walls[4].setRectValues(2126, 1949, 61, 17);
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

