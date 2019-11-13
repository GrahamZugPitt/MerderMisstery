#ifndef _YELLOWBUILDING_HPP__
#define _YELLOWBUILDING_HPP__

#include "objects.hpp"
#include "common.hpp"

class YellowBuilding{
    public:
        YellowBuilding();
        ~YellowBuilding();
        bool checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap);

    private:
        SDL_Rect rect = {835, 1202, 708, 888};
        Collidable walls[6];
};

#endif
