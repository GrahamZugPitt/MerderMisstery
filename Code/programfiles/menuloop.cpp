#include "gameloop.hpp"
#include "main_helper.hpp"
#include "chat.hpp"
#include "seed.hpp"
#include "credits.hpp"

std::string logoImgPath = "Art/Logo/Start_Screen.png";
std::string loadImgPath = "Art/Logo/Loading.png";

const int ALL_BUTTON_TOP = 618;
const int ALL_BUTTON_BOTTOM = 532;

void menuloop(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  // Load the title screens
  SDL_Texture* logoScreen = loadFiles(logoImgPath, renderer);
  bool next = false;

  int cursor_x;
  int cursor_y;

  //Logo Screen Loop
  while(!next && !(*quit)) {

    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT) {
        (*quit) = true;
      }
    }

    // if mouse is clicked and released
    if(e.type == SDL_MOUSEBUTTONUP) {
      // Get mouse position
      cursor_x = e.motion.x;
      cursor_y = e.motion.y;

      // ALL_BUTTON_TOP Y value: 532
      // ALL_BUTTON_BOTTOM Y value: 618
      // check if click was in Y range of buttons
      // if it wasn't in Y range, no need to check X range
      if((cursor_y > ALL_BUTTON_BOTTOM) && (cursor_y < ALL_BUTTON_TOP))
      {
        // cursor_x value comparisons to button ranges
        switch(cursor_x){
          //Enter Seed Left-X value: 107
          //Enter Seed Right-X value: 393
          case 107 ... 393:
            SDL_Delay(300);
            enter_seed(e, &(*quit), keyState, renderer);
            SDL_Delay(300);
            std::cout << "Exited Seed Loop\n";
            break;

          //Start Game Left-X value: 492
          //Start Game Right-X value: 779
          case 492 ... 779:
            std::cout << "Start Game\n";
            next = true; // Start the game
            break;

          //Roll Credits Left-X value: 876
          //Roll Credits Right-X value: 1164
          case 876 ... 1164:
            std::cout << "End Scene\n";
            roll_credits(e, &(*quit), keyState, renderer);
            break;
        }
      }
    }

    // Get the Keyboard State
    keyState = SDL_GetKeyboardState(NULL);

    // Open Chat room
    if (keyState[SDL_SCANCODE_C]) {
      SDL_Delay(300);
      enter_chat(e, &(*quit), keyState, renderer);
      SDL_Delay(300);
      std::cout << "Exited Chat Loop\n";
    }

    //Logo Screen Render
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, logoScreen, NULL, NULL);
    SDL_RenderPresent(renderer);
  }

  // Obviously don't want to do any work if we're quitting
  if(!*quit){
    SDL_Texture* loadScreen = loadFiles(loadImgPath, renderer);
    SDL_RenderCopy(renderer, loadScreen, NULL, NULL);
    SDL_RenderPresent(renderer);
  }
}
