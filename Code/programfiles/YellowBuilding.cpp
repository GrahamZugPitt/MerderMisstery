#include "YellowBuilding.hpp"

YellowBuilding::YellowBuilding(){
    walls[0].setRectValues(1366, 1816, 187, 16);
    walls[1].setRectValues(1534, 1213, 18, 617);
    walls[2].setRectValues(879, 1210, 672, 17);
    walls[3].setRectValues(878, 1212, 17, 618);
    walls[4].setRectValues(877, 1816, 368, 16);
    walls[5].setRectValues(1235, 1213, 20, 617);
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

