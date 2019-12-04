#include "RedBuilding.hpp"

RedBuilding::RedBuilding(){
    walls[0].setRectValues(2015, 519, 355, 15);
    walls[1].setRectValues(2351, 38, 20, 496);
    walls[2].setRectValues(1504, 37, 866, 16);
    walls[3].setRectValues(1505, 39, 16, 497);
    walls[4].setRectValues(1505, 510, 377, 15);
    walls[5].setRectValues(1488, 300, 348, 10);
    walls[6].setRectValues(1990, 300, 345, 10);
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

