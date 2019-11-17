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
        SDL_Rect rect = {1449, 22, 909, 808};
        Collidable walls[7];
};

#endif
