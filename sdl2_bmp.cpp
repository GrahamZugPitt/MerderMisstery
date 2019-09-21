#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>

#include <typeinfo>

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

// Function declarations
bool init();
SDL_Surface* loadImage(std::string fname);
void close();

// Globals
SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* gImg = nullptr;

bool init() {	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}
	
	gWindow = SDL_CreateWindow("CS 1666...", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		std::cout << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
		return  false;
	}
	
	gScreenSurface = SDL_GetWindowSurface(gWindow);
	return true;
}

SDL_Surface* loadImage(std::string fname) {
	SDL_Surface* optSurf = nullptr;

	SDL_Surface* startSurf = SDL_LoadBMP(fname.c_str());
	if (startSurf == nullptr) {
		std::cout << "Unable to load image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
		return nullptr;
	}
		
	optSurf = SDL_ConvertSurface(startSurf, gScreenSurface->format, 0);
	if (optSurf == nullptr) {
		std::cout << "Unable to optimize image " << fname << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(startSurf);

	return optSurf;
}

void close() {
	SDL_FreeSurface(gImg);
	gImg = nullptr;

	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;

	// Quit SDL subsystems
	SDL_Quit();
}

int main() {
	if (!init()) {
		std::cout <<  "Failed to initialize!" << std::endl;
		close();
		return 1;
	}

	std::string dir = "Credit Pictures/";
	std::string ext = "CreditPicture.png";
	std::vector <std::string> imgstrs{"Alex", "Bove", "Brindock", "Bryan", "Darpun", "dves", "Griffin", "Jahnke", "Jiang", "ShawnBlake", "Zug"};
	for(auto name: imgstrs){
		std::string full = dir + name + ext;
		// Using IMG_Load to load png images
		// NOTE: the total lack of error checking
		gImg = IMG_Load(full.c_str());
		// Apply the image
		SDL_BlitSurface(gImg, NULL, gScreenSurface, NULL );
		// Update the window surface
		SDL_UpdateWindowSurface(gWindow);
		// Wait 5 seconds
		SDL_Delay(1000);
	}
	
	
	close();
}
