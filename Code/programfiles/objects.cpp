#include "objects.hpp"

// Chair methods
Chair::Chair(){};

Chair::Chair(int xPos, int yPos){
	Collidable::rect.x = xPos;
	Collidable::rect.y = yPos;
	Collidable::rect.w = CHAIR_WIDTH;
	Collidable::rect.h = CHAIR_HEIGHT;
};

Chair::~Chair(){};

void Chair::setPos(int x, int y){
	Collidable::setRectValues(x+5, y, CHAIR_WIDTH-10, CHAIR_HEIGHT-25);
}

// Table methods
Table::Table(){};
Table::~Table(){};

void Table::setPos(int x, int y){
	Collidable::setRectValues(x, y, 81, 46);
	// Collidable::addChild(x, y, 46, 81);
	// Collidable::addChild(x+4, y+46, 9, 25);
	// Collidable::addChild(x+68, y+46, 9, 25);
}