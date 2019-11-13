#ifndef _CHAIR_HPP__
#define _CHAIR_HPP__

#include "collision.hpp"

constexpr int CHAIR_WIDTH = 36;
constexpr int CHAIR_HEIGHT = 45;

class Chair: public Collidable
{
public:
	Chair();
	Chair(int xPos, int yPos);
	~Chair();

	void setPos(int x, int y);
};

#endif