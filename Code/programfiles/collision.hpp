#ifndef COLLISION_H__
#define COLLISION_H__

#include "common.hpp"
#include <vector>

class Collidable
{
public:
	Collidable();
	Collidable(SDL_Rect bounds);
	~Collidable();
	
	std::string toString();

	bool checkCollision(SDL_Rect *playerRect, SDL_Rect *Overlap);

	SDL_Rect rect;
	std::vector<Collidable> children;

	void addChild(Collidable child);
};

#endif