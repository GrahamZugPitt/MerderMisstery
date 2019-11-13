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
Collidable::Collidable(int x, int y, int w, int h){
	rect.x = x;
	rect.y = y;
	rect.w = w;
	rect.h = h;
}
Collidable::~Collidable(){
	if(!children.empty())
		children.clear();
}

void Collidable::setRectValues(int xPos, int yPos, int width, int height){
	rect.x = xPos;
	rect.y = yPos;
	rect.w = width;
	rect.h = height;
}

Collidable& Collidable::operator=( const Collidable& other){
	rect.x = other.rect.x;
	rect.y = other.rect.y;
	rect.w = other.rect.w;
	rect.h = other.rect.h;
	for (auto child: other.children){
		children.push_back(child);
	}
	return *this;
}

void Collidable::addChild(Collidable child){
	if(child.rect.x < rect.x || child.rect.y < rect.y ||
		child.rect.x + child.rect.w > rect.x + rect.w ||
		child.rect.y + child.rect.h > rect.y + rect.h ){
		std::cerr << "WARNING child bounds exceed parent's\n" << std::endl;
	}
	children.push_back(child);
}

void Collidable::addChild(int x, int y, int w, int h){
	Collidable child(x, y, w, h);
	if(child.rect.x < rect.x || child.rect.y < rect.y ||
		child.rect.x + child.rect.w > rect.x + rect.w ||
		child.rect.y + child.rect.h > rect.y + rect.h ){
		std::cerr << "WARNING child bounds exceed parent's\n" << std::endl;
	}
	children.push_back(child);
}

bool Collidable::checkCollision(SDL_Rect *playerRect, SDL_Rect *overlap){
	bool found = false;
	if(SDL_HasIntersection(&rect, playerRect)){
		if(children.empty()){
			found = true;
			SDL_IntersectRect(&rect, playerRect, overlap);
		} else {
			for(auto child: children){
				if ( SDL_HasIntersection(&(child.rect), playerRect) ){
					SDL_IntersectRect(&(child.rect), playerRect, overlap);
					found = true;
					break;
				}
			}
		}
	}
	return found;
}
/* Not working properly
int Collidable::checkCollisionLevel(SDL_Rect *playerRect, SDL_Rect *overlap, int *level, int  searchDepth){
	if(SDL_HasIntersection(&rect, playerRect)){
		*level += 1;
		//printf("found match level: %d", *level);
		if (*level ==  searchDepth){
			SDL_IntersectRect(&rect, playerRect, overlap);
			//return *level;
		} else {
			for (auto child: children){
				child.checkCollisionLevel(playerRect, overlap, level,  searchDepth);
				break;
			}
		}
	}
	return *level;
}
*/
std::string Collidable::toString(){
	std::string s;
	s += "Collidable: Rect (" + std::to_string(rect.x) + ", " + std::to_string(rect.y) + ", " + std::to_string(rect.w)+ ", " + std::to_string(rect.h)+")";
	if(!children.empty()){
		s += "\n  Children:\n";
		for(auto child: children){
			s+= child.toString();
		}
		s += "\n End Children\n";
	}
	return s;
}
// Used for testing
/*
#ifdef TESTING
int main(int argc, char** argv){
	printf("Main running\n");
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 100;
	rect.h = 100;
	Collidable c(rect);
	rect.x = 0;
	rect.y = 0;
	rect.w = 50;
	rect.h = 100;
	Collidable c2(rect);
	rect.x = 0;
	rect.y = 0;
	rect.w = 50;
	rect.h = 50;
	Collidable c3(rect);
	c2.addChild(c3);
	c.addChild(c2);
	//c.addChild(c3);
	SDL_Rect player;
	SDL_Rect intersect;
	player.x = 0;
	player.y = 60;
	player.w = 100;
	player.h = 100;
	std::cout << c.toString() << std::endl;
	//std::cout << c.children[0].toString() << std::endl;
	int level = 0;
	std::cout << c.checkCollision(&player, &intersect) << std::endl;


	//std::cout << intersect.x << ", " << intersect.y << ", " << intersect.w << ", " << intersect.h << std::endl;
	level = 0;
	int  searchDepth = 3;	
	std::cout << (searchDepth == c.checkCollisionLevel(&player, &intersect, &level,  searchDepth)) << std::endl;
	std::cout << intersect.x << ", " << intersect.y << ", " << intersect.w << ", " << intersect.h << std::endl;
	
}
#endif
*/