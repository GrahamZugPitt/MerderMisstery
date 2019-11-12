#include "credits.hpp"
#include "main_helper.hpp"

SDL_Texture* gImg = nullptr;

void roll_credits(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  // inCredits boolean value
  // bool inCredits = true;

  std::string dir = "Art/Credit Pictures/";
	std::string ext = "CreditPicture.png";
	std::vector <std::string> imgstrs{"Alex", "Bove", "Brindock", "Bryan",
    "Darpun", "dves", "Griffin", "Jahnke", "Jiang", "ShawnBlake", "Zug"};
	for(auto name: imgstrs){
		std::string full = dir + name + ext;

    // load image files
		gImg = loadFiles(full.c_str(), renderer);

    SDL_RenderCopy(renderer, gImg, NULL, NULL);
    SDL_RenderPresent(renderer);

		// Wait 3 seconds
		SDL_Delay(3000);

    // //Get the Keyboard State
    // keyState = SDL_GetKeyboardState(NULL);
    //
    // // If user selects escape or enter keys, leave credits
    // if (keyState[SDL_SCANCODE_ESCAPE] || keyState[SDL_SCANCODE_RETURN])
    //   return;
    //
    // //Quit application
    // if(e.type == SDL_QUIT)
    //   *quit = true;

	}

}
