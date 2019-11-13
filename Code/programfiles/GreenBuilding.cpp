#include "GreenBuilding.hpp"

GreenBuilding::GreenBuilding(){
    walls[0].setRectValues(2257, 1958, 679, 10);
    walls[1].setRectValues(2926, 1474, 10, 489);
    walls[2].setRectValues(2085, 1472, 851, 10);
    walls[3].setRectValues(2085, 1472, 10, 492);
    walls[4].setRectValues(2085, 1955, 60, 10);
}

bool GreenBuilding::checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap){
    if(!SDL_HasIntersection(&rect, playerRect)){
        return false;
    } else {
        for(auto wall: walls){
            if (wall.checkCollision(playerRect, overlap)){
                std::cout << "wall collision" << overlap->x << " " << overlap->y << " " << overlap->w << " " << overlap->h << std::endl;
                return true;
            }
        }
    }
    return false;
}

GreenBuilding::~GreenBuilding(){}

