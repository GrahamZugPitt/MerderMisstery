#include "RedBuilding.hpp"

RedBuilding::RedBuilding(){
    walls[0].setRectValues(1971, 526, 366, 10);
    walls[1].setRectValues(2329, 46, 10, 489);
    walls[2].setRectValues(1488, 46, 849, 10);
    walls[3].setRectValues(1488, 46, 10, 489);
    walls[4].setRectValues(1488, 528, 366, 10);
    walls[5].setRectValues(1488, 300, 348, 10);
    walls[6].setRectValues(1990, 300, 345, 10);
}

bool RedBuilding::checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap){
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

RedBuilding::~RedBuilding(){}

