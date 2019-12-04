#ifndef _REDBUILDING_HPP__
#define _REDBUILDING_HPP__

#include "objects.hpp"
#include "common.hpp"

class RedBuilding{
    public:
        RedBuilding();
        ~RedBuilding();
        bool checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap);

    private:
        SDL_Rect rect = {1498, 30, 880, 512};
        Collidable walls[13];
};

#endif
