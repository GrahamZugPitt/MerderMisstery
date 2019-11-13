/*  building.cpp
*		by: Erick Brindock
*	A c++ class to represent a building object on the map
*	
*	Right now it simply holds and renders a sprite to represent the building,
*	as well as perfomring the collision detection on the walls. I intend to make it
*	hold other WorldObjects as well. Then their collision detection will only happen
*	if the player is in the building. Additionally, the object sprites can be placed
*	on the buildings texture saving some draws if necessary.
*
*	It extends the Collidable class to enable simple or "striped" collision detection.
*/

#include "Building.hpp"
// Default constructor
Building::Building() :WorldObject(){};

/* Creates a Building object
	Args:
		spriteFile: the path and name of the sprite to use
		*renderer: the SDL_Renderer being used
		mapX: the x position on the map
		mapY: the y position on the map
		mapW: the width of the sprite
		mapH: the height of the sprite
		colX: the x position of the collision box (usually same as mapX)
		colY: the y position of the collision box (usually same as mapY)
		colW: the width of the collision box (usually the same as mapW)
		colH: the height of the collision box (usually the same as mapH)
*/
Building::Building(std::string spriteFile, SDL_Renderer *renderer, int mapX, int mapY, int mapW, int mapH,int colX, int colY, int colW, int colH)
	:WorldObject(spriteFile, renderer, mapX, mapY, mapW, mapH, colX, colY, colW, colH){};
Building::~Building(){};

/* Adds the collision boxes for the walls
	Args:
		width: the width (thickness) of the walls
		door: the location and width of the door relative to the sprite
				If the door is on the east/west, only y and height really matter.
				If the door is on the north/south, only x and width matter.
		doorFacing: the wall the door is on (NORTH, SOUTH, EAST, WEST)
	For example, adding walls to a 10x10 building with args (1, {5,0,3,0}, EAST) 
	will make one like this where each dash is one pixel:
	 + - - - - - - - - +
	 |                 |
	 |                 |
	 |                 |
	 |                 
	 |                 
	 |                 
	 |                 |
	 |                 |
	 + - - - - - - - - +
*/
void Building::addWalls(int width, SDL_Rect door, int doorFacing){
	switch(doorFacing){
		case NORTH:{
			std::cout << "North" << std::endl;
			Collidable n(WorldObject::mapPos.x, WorldObject::mapPos.y, door.x, width);
			Collidable fromdoor(WorldObject::mapPos.x + door.w + door.x, WorldObject::mapPos.y, WorldObject::mapPos.w - door.w - door.x, width);
			Collidable s(WorldObject::mapPos.x, WorldObject::mapPos.y + WorldObject::mapPos.h - width, WorldObject::mapPos.w, width);
			Collidable e(WorldObject::mapPos.x + WorldObject::mapPos.w - width, WorldObject::mapPos.y + width,width, WorldObject::mapPos.h - 2*width);
			Collidable w(WorldObject::mapPos.x, WorldObject::mapPos.y + width, width, WorldObject::mapPos.h - 2*width);
			std::cout << "North" << std::endl;
			Collidable::addChild(n);
			std::cout << "South" << std::endl;
			Collidable::addChild(s);
			std::cout << "East" << std::endl;
			Collidable::addChild(e);
			std::cout << "West" << std::endl;
			Collidable::addChild(w);
			std::cout << "Door" << std::endl;
			Collidable::addChild(fromdoor);
			break;
		}
		case SOUTH:{
			std::cout << "South" << std::endl;
			Collidable n(WorldObject::mapPos.x, WorldObject::mapPos.y, WorldObject::mapPos.w, width);
			Collidable s(WorldObject::mapPos.x, WorldObject::mapPos.y + WorldObject::mapPos.h - width, door.x, width);
			Collidable fromdoor(WorldObject::mapPos.x + door.w + door.x, WorldObject::mapPos.y + WorldObject::mapPos.h - width, WorldObject::mapPos.w - door.w - door.x, width);
			Collidable e(WorldObject::mapPos.x + WorldObject::mapPos.w - width, WorldObject::mapPos.y + width,width, WorldObject::mapPos.h - 2*width);
			Collidable w(WorldObject::mapPos.x, WorldObject::mapPos.y + width, width, WorldObject::mapPos.h - 2*width);
			std::cout << "North" << std::endl;
			Collidable::addChild(n);
			std::cout << "South" << std::endl;
			Collidable::addChild(s);
			std::cout << "East" << std::endl;
			Collidable::addChild(e);
			std::cout << "West" << std::endl;
			Collidable::addChild(w);
			std::cout << "Door" << std::endl;
			Collidable::addChild(fromdoor);
			break;
		}
		case EAST:{
			std::cout << "East" << std::endl;
			Collidable n(WorldObject::mapPos.x, WorldObject::mapPos.y, WorldObject::mapPos.w, width);
			Collidable s(WorldObject::mapPos.x, WorldObject::mapPos.y + WorldObject::mapPos.h - width, WorldObject::mapPos.w, width);
			Collidable e(WorldObject::mapPos.x + WorldObject::mapPos.w - width, WorldObject::mapPos.y,width, door.y-width);
			Collidable fromdoor(WorldObject::mapPos.x + WorldObject::mapPos.w - width, WorldObject::mapPos.y + door.h + door.y, width, WorldObject::mapPos.h - door.y - door.h - width);
			Collidable w(WorldObject::mapPos.x, WorldObject::mapPos.y + width, width, WorldObject::mapPos.h - 2*width);
			std::cout << "North" << std::endl;
			Collidable::addChild(n);
			std::cout << "South" << std::endl;
			Collidable::addChild(s);
			std::cout << "East" << std::endl;
			Collidable::addChild(e);
			std::cout << "West" << std::endl;
			Collidable::addChild(w);
			std::cout << "Door" << std::endl;
			Collidable::addChild(fromdoor);
			break;
		}
		case WEST:{
			std::cout << "West" << std::endl;
			Collidable n(WorldObject::mapPos.x, WorldObject::mapPos.y, WorldObject::mapPos.w, width);
			Collidable s(WorldObject::mapPos.x, WorldObject::mapPos.y + WorldObject::mapPos.h - width, WorldObject::mapPos.w, width);
			Collidable e(WorldObject::mapPos.x + WorldObject::mapPos.w - width, WorldObject::mapPos.y + width,width, WorldObject::mapPos.h - 2*width);
			Collidable w(WorldObject::mapPos.x, WorldObject::mapPos.y + width, width, door.y-width);
			Collidable fromdoor(WorldObject::mapPos.x, WorldObject::mapPos.y + door.y + door.h, width, WorldObject::mapPos.h - door.y - door.h - width);
			std::cout << "North" << std::endl;
			Collidable::addChild(n);
			std::cout << "South" << std::endl;
			Collidable::addChild(s);
			std::cout << "East" << std::endl;
			Collidable::addChild(e);
			std::cout << "West" << std::endl;
			Collidable::addChild(w);
			std::cout << "Door" << std::endl;
			Collidable::addChild(fromdoor);
			break;
		}
	}
};