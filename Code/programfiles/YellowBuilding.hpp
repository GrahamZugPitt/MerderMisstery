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
        SDL_Rect rect = {870, 1203, 689, 638};
        Collidable walls[7];
};

#endif
