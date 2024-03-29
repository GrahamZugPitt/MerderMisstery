#ifndef COLLISION_H__
#define COLLISION_H__

#include "common.hpp"
#include <vector>

class Collidable
{
public:
	Collidable();
	Collidable(SDL_Rect bounds);
	Collidable(int x, int y, int w, int h);
	~Collidable();

	Collidable& operator=( const Collidable& other);
	
	std::string toString();

	bool checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap);

	//int checkCollisionLevel(SDL_Rect *playerRect, SDL_Rect *overlap, int *level, int  searchDepth);
	SDL_Rect rect;
	std::vector<Collidable> children;

	void addChild(Collidable child);
	void addChild(int x, int y, int w, int h);

	void setRectValues(int xPos, int yPos, int width, int height);

};

#endif