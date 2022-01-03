#include "BlueBuilding.hpp"

BlueBuilding::BlueBuilding(){
    walls[0].setRectValues(2392, 735, 16, 202);
    walls[1].setRectValues(2408, 735, 247, 17);
    walls[2].setRectValues(2655, 438, 16, 314);
    walls[3].setRectValues(2671, 438, 232, 18);
    walls[4].setRectValues(2903, 438, 17, 771);
    walls[5].setRectValues(2671, 1191, 232, 18);
    walls[6].setRectValues(2654, 920, 17, 289);
    walls[7].setRectValues(2539, 920, 115, 16);
    walls[8].setRectValues(2408, 920, 51, 16);
}

bool BlueBuilding::checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap){
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

BlueBuilding::~BlueBuilding(){}

