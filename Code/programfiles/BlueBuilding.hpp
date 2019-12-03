#ifndef _BLUEBUILDING_HPP__
#define _BLUEBUILDING_HPP__

#include "objects.hpp"
#include "common.hpp"

class BlueBuilding{
    public:
        BlueBuilding();
        ~BlueBuilding();
        bool checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap);

    private:
        SDL_Rect rect = {2357, 398, 561, 878};
        Collidable walls[9];
};

#endif
