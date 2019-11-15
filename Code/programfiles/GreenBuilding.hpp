#ifndef _GREENBUILDING_HPP__
#define _GREENBUILDING_HPP__

#include "objects.hpp"
#include "common.hpp"

class GreenBuilding{
    public:
        GreenBuilding();
        ~GreenBuilding();
        bool checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap);

    private:
        SDL_Rect rect = {1542, 1278, 1416, 801};
        Collidable walls[5];
};

#endif
