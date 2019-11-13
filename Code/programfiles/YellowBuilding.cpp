#include "YellowBuilding.hpp"

YellowBuilding::YellowBuilding(){
    walls[0].setRectValues(1337, 1825, 180, 10);
    walls[1].setRectValues(1520, 1219, 10, 611);
    walls[2].setRectValues(854, 1219, 676, 10);
    walls[3].setRectValues(854, 1219, 10, 607);
    walls[4].setRectValues(854, 1825, 369, 10);
    walls[5].setRectValues(1197, 1220, 10, 610);
}

bool YellowBuilding::checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap){
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

YellowBuilding::~YellowBuilding(){}

