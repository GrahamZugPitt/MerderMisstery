#include "RedBuilding.hpp"

RedBuilding::RedBuilding(){
    walls[0].setRectValues(1504, 37, 16, 499);
    walls[1].setRectValues(1520, 37, 835, 16);
    walls[2].setRectValues(2355, 37, 17, 498);
    walls[3].setRectValues(1977, 519, 378, 16);
    walls[4].setRectValues(1520, 519, 377, 17);
    walls[5].setRectValues(1520, 296, 341, 17);
    walls[6].setRectValues(2014, 296, 341, 17);
    walls[7].setRectValues(1865, 53, 15, 63);
    walls[8].setRectValues(1994, 53, 16, 63);
    walls[9].setRectValues(1999, 259, 15, 93);
    walls[10].setRectValues(1999, 472, 15, 47);
    walls[11].setRectValues(1861, 471, 15, 48);
    walls[12].setRectValues(1861, 259, 15, 93);
}

bool RedBuilding::checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap){
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

RedBuilding::~RedBuilding(){}

