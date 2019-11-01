#include "common.hpp"
#include "collision.hpp"

Collidable::Collidable(){
	rect.x = 0;
	rect.y = 0;
	rect.w = 0;
	rect.h = 0;
}
Collidable::Collidable(SDL_Rect bounds){
	rect.x = bounds.x;
	rect.y = bounds.y;
	rect.w = bounds.w;
	rect.h = bounds.h;
}
Collidable::~Collidable(){
	if(!children.empty())
		children.clear();
}
void Collidable::addChild(Collidable child){
	children.push_back(child);
}

bool Collidable::checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap){
	bool hasIntersect = false;
	if(SDL_IntersectRect(&rect, playerRect, overlap)){
		hasIntersect = true;
		for(auto child: children){
			hasIntersect = child.checkCollision(playerRect, overlap);
			if (hasIntersect){
				break;
			}
		}
	}
	return hasIntersect;
}

std::string Collidable::toString(){
	std::string s;
	s += "Collidable: Rect (" + std::to_string(rect.x) + ", " + std::to_string(rect.y) + ", " + std::to_string(rect.w)+ ", " + std::to_string(rect.h)+")";
	for(auto child: children){
		s+= child.toString();
	}
	return s;
}
// Used for testing
#ifdef TESTING
int main(int argc, char** argv){
	printf("Main running\n");
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 100;
	rect.h = 100;
	Collidable c(rect);
	rect.x = 10;
	rect.y = 10;
	rect.w = 80;
	rect.h = 80;
	Collidable c2(rect);
	c.addChild(c2);

	SDL_Rect player;
	SDL_Rect intersect;
	player.x = 70;
	player.y = 70;
	player.w = 100;
	player.h = 100;
	std::cout << c.toString() << std::endl;
	std::cout << c.children[0].toString() << std::endl;
	std::cout << c.checkCollision(&player, &intersect) << std::endl;

	std::cout << intersect.x << ", " << intersect.y << ", " << intersect.w << ", " << intersect.h << std::endl;
}
#endif