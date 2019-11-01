#ifndef BUILDING_H__
#define BUILDING_H__

#include <SDL2/SDL_image.h>
#include <string>
#include <vector>

class Building
{
public:
	SDL_Texture *texture;
	SDL_Rect mapPos;
	#ifdef OBJ_IMPLEMENTED
	std::vector worldObject interiorItems;
	#endif
	Building();
	~Building();
	bool checkCollisions(SDL_Rect *overlap);
	int initSprite(std::string imageName, int posX, int posY);
};

#endif