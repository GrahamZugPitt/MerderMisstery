#ifndef _SIGN1_HPP__
#define _SIGN1_HPP__

#include "objects.hpp"
#include "common.hpp"

class Sign1{
public:
	Sign1();
	~Sign1();
	bool checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap);


private:
	SDL_Rect rect = {911, 365, 557, 750};
	Collidable walls[8];
};

#endif
