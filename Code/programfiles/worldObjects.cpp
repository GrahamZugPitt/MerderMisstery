/*	A C++ class representing an object in the world for the MurderMistery game
*		by: Erick Brindock
*
*	This class holds a sprite to draw as well as the position information needed to 
* 	display it.
*	
*	It extends the Collidable class to enable simple or "striped" collision detection.
*
*/

#include "worldObjects.hpp"


SDL_Rect castlePosition = {1915, 815, 38, 60};

// This is a pretty basic one. Should be more code for NPCs
SDL_Rect getCastle(){
    return castlePosition;
}
// Deafult constructor 
// set everything to null or zero
WorldObject::WorldObject() :Collidable(0,0,0,0), texture{NULL} {
	mapPos.x = 0;
	mapPos.y = 0;
	mapPos.w = 0;
	mapPos.h = 0;

	screenPos.x = 0;
	screenPos.y = 0;
	screenPos.w = 0;
	screenPos.h = 0;
	crop.x = 0;
	crop.y = 0;
	crop.w = 0;
	crop.h = 0;
};
/* Creates a WorldObject and initializes it.
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
WorldObject::WorldObject(std::string spriteFile, SDL_Renderer *renderer, int mapX, int mapY, int mapW, int mapH,int colX, int colY, int colW, int colH)
:Collidable(colX, colY, colW, colH) {
	int code = 0;
	if ((code = initObject(spriteFile, renderer, mapX, mapY, mapW, mapH, colX, colY, colW, colH)) != 0)
		std::cout << "Object not created Error: "<< code <<std::endl;
		
	std::cout << "World Object Created" <<std::endl;
	std::cout << "map " << mapPos.x << ", " << mapPos.y << ", " << mapPos.w << ", " << mapPos.h << std::endl;

	std::cout << "crop " << crop.x << ", " << crop.y << ", " << crop.w << ", " << crop.h << std::endl;

	std::cout << "screen " << screenPos.x << ", " << screenPos.y << ", " << screenPos.w << ", " << screenPos.h << std::endl;

	std::cout << Collidable::toString() << std::endl;
}

/* Initializes a world object. This load an image and converts it to a texture to render.
	It also sets up the renderign and collision information for the object.
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
int WorldObject::initObject(std::string spriteFile, SDL_Renderer *renderer, int mapX, int mapY, int mapW, int mapH,int colX, int colY, int colW, int colH){
	SDL_Surface *surface = IMG_Load(spriteFile.c_str());
    //Check if the sprite sheet is loaded
    if (surface == NULL){
        std::cout << "Error loading Image!\n";
        return -1;
    } else {
        //Create texture from player sprite surface we just created
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        
        //Check if texture was able to be made
        if (texture == NULL){
			std::cout << "Error creating texture\n";
        	return -2;
        }
    }

    mapPos.x = mapX;
	mapPos.y = mapY;
	mapPos.w = mapW;
	mapPos.h = mapH;

	Collidable::rect.x = colX;
	Collidable::rect.y = colY;
	Collidable::rect.w = colW;
	Collidable::rect.h = colH;


	screenPos.x = 0;
	screenPos.y = 0;
	screenPos.w = mapW;
	screenPos.h = mapH;

	crop.x = 0;
	crop.y = 0;
	crop.h = mapH;
	crop.w = mapW;
    return 0;
}

/* Renders the object to the screen.
	Args:
		renderer: the renderer being used
		camPos: the rect of the camera
*/
void WorldObject::renderToScreen(SDL_Renderer *renderer, SDL_Rect camPos ){
	if(renderer != NULL && texture != NULL){
		screenPos.x = mapPos.x - camPos.x;
		screenPos.y = mapPos.y - camPos.y;
		if (screenPos.x >= -screenPos.w && screenPos.x <= 1280 + screenPos.w &&
			screenPos.y >= -screenPos.h && screenPos.y <= 720 + screenPos.h)
			SDL_RenderCopy(renderer, texture, &crop, &screenPos);
	}
}

WorldObject::~WorldObject(){
	if (texture != NULL){
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
}