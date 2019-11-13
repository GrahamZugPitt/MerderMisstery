#include "BlueBuilding.hpp"

BlueBuilding::BlueBuilding(){
    walls[0].setRectValues(2531, 932, 118, 10);
    walls[1].setRectValues(2641, 940, 10, 259);
    walls[2].setRectValues(2642, 1190, 242, 10);
    walls[3].setRectValues(2877, 451, 10, 744);
    walls[4].setRectValues(2641, 450, 244, 10);
    walls[5].setRectValues(2641, 450, 10, 303);
    walls[6].setRectValues(2369, 747, 278, 10);
    walls[7].setRectValues(2369, 747, 10, 188);
    walls[8].setRectValues(2369, 932, 50, 10);
}

bool BlueBuilding::checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap){
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

BlueBuilding::~BlueBuilding(){}

