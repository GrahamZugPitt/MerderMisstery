#include "credits.hpp"
#include "main_helper.hpp"

SDL_Texture* gImg = nullptr;

void roll_credits(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){

  std::string dir = "Art/Credit Pictures/";
	std::string ext = "CreditPicture.png";
	std::vector <std::string> imgstrs{"Alex", "Bove", "Brindock", "Bryan",
    "Darpun", "dves", "Griffin", "Jahnke", "Jiang", "ShawnBlake", "Zug"};
	for(auto name: imgstrs){
		std::string full = dir + name + ext;
		// Using IMG_Load to load png images
		// NOTE: the total lack of error checking
		gImg = loadFiles(full.c_str(), renderer);
		// Apply the image
    SDL_RenderCopy(renderer, gImg, NULL, NULL);
		// SDL_BlitSurface(gImg, NULL, gScreenSurface, NULL );
		// Update the window surface
		// SDL_UpdateWindowSurface(gWindow);
    SDL_RenderPresent(renderer);
		// Wait 5 seconds
		SDL_Delay(3000);
	}

}
