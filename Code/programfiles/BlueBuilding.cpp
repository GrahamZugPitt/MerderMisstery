#include "BlueBuilding.hpp"

BlueBuilding::BlueBuilding(){
    walls[0].setRectValues(2560, 918, 70, 18);
    walls[1].setRectValues(2653, 921, 18, 288);
    walls[2].setRectValues(2655, 1189, 264, 17);
    walls[3].setRectValues(2902, 441, 17, 767);
    walls[4].setRectValues(2655, 439, 262, 15);
    walls[5].setRectValues(2654, 439, 15, 313);
    walls[6].setRectValues(2393, 734, 277, 17);
    walls[7].setRectValues(2392, 737, 18, 198);
    walls[8].setRectValues(2392, 917, 40, 19);
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

