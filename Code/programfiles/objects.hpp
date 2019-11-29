#ifndef _OBJECTS_HPP__
#define _OBJECTS_HPP__

#include "collision.hpp"

constexpr int CHAIR_WIDTH = 36;
constexpr int CHAIR_HEIGHT = 45;

constexpr int TABLE_WIDTH = 81;

class Chair: public Collidable
{
public:
	Chair();
	Chair(int xPos, int yPos);
	~Chair();

	void setPos(int x, int y);
};

class Table: public Collidable
{
public:
	Table();
	~Table();

	void setPos(int x, int y);
};

#endif