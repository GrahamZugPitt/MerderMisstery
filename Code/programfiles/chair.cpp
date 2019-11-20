#include "chair.hpp"

Chair::Chair(){};

Chair::Chair(int xPos, int yPos){
	Collidable::rect.x = xPos;
	Collidable::rect.y = yPos;
	Collidable::rect.w = CHAIR_WIDTH;
	Collidable::rect.h = CHAIR_HEIGHT;
};

Chair::~Chair(){};

void Chair::setPos(int x, int y){
	Collidable::setRectValues(x, y, CHAIR_WIDTH, CHAIR_HEIGHT);
}