#include "menuloop.hpp"

std::string logoImgPath = "Art/Logo/Start_Screen.png";
std::string loadImgPath = "Art/Logo/Loading.png";
SDL_Texture* logoScreen;

const int ALL_BUTTON_TOP = 532;
const int ALL_BUTTON_BOTTOM = 618;
const int I_BUTTON_TOP = 42;
const int I_BUTTON_BOTTOM = 102;
const int I_BUTTON_LEFT = 1070;
const int I_BUTTON_RIGHT = 1124;
bool chatLoggedIn = false;

std::fstream save;

std::string menuloop(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  render_menu(renderer);

  bool next = false;

  int cursor_x;
  int cursor_y;

  vector<string> credentials;

  std::string seed = "";
  SDL_Event nullEvent;

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
      if((cursor_y < ALL_BUTTON_BOTTOM) && (cursor_y > ALL_BUTTON_TOP))
      {
        // cursor_x value comparisons to button ranges
        switch(cursor_x){
          //Enter Seed Left-X value: 107
          //Enter Seed Right-X value: 393
          case 107 ... 393:
            if (save) {  // Replace the seed call to the abort
              remove("save.txt");
              logoImgPath = "Art/Logo/Start_Screen.png";
              logoScreen = loadFiles(logoImgPath, renderer);
              e = nullEvent;
              save.open("save.txt", std::fstream::in);
              save.close();
              render_menu(renderer);
            } else {
              seed = enter_seed(e, &(*quit), keyState, renderer);
              // If the player entered a seed, start the game
              if(seed.compare("") != 0){
                next = true;
                //clear logoScreen render
                SDL_DestroyTexture(logoScreen);
              }
              std::cout << "Exited Seed Loop\n";
            }
            break;

          //Start Game Left-X value: 492
          //Start Game Right-X value: 779
          case 492 ... 779:
              std::cout << "Start Game\n";
              next = true; // Start the game
              //clear logoScreen render
              SDL_DestroyTexture(logoScreen);
            break;

          //Roll Credits Left-X value: 876
          //Roll Credits Right-X value: 1164
          case 876 ... 1164:
            std::cout << "End Scene\n";
            //clear logoScreen render
            SDL_DestroyTexture(logoScreen);

            roll_credits(e, &(*quit), keyState, renderer);

            //Logo Screen Render
            render_menu(renderer);
            break;
        }
      }
      // check if click was in XY range of buttons
      else if((cursor_y < I_BUTTON_BOTTOM) && (cursor_y > I_BUTTON_TOP) &&
      (cursor_x > I_BUTTON_LEFT) && (cursor_x < I_BUTTON_RIGHT))
      {
        std::cout << "open information\n";

        //clear logoScreen render
        SDL_DestroyTexture(logoScreen);
        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);

        info_page(e, &(*quit), keyState, renderer);

        //Logo Screen Render
        render_menu(renderer);
      }
    }

    // Get the Keyboard State
    keyState = SDL_GetKeyboardState(NULL);

    // Open Chat room
    if (keyState[SDL_SCANCODE_C]) {
      if(!chatLoggedIn) {
        //clear logoScreen render
        SDL_DestroyTexture(logoScreen);

        credentials = enter_login(e, &(*quit), keyState, renderer);
        chatLoggedIn=true;
        std::cout << "Exited Chat Login Loop\n";

        //Logo Screen Render
        render_menu(renderer);
      }
      //clear logoScreen render
      SDL_DestroyTexture(logoScreen);

      enter_chat(e, &(*quit), keyState, renderer, credentials);
      std::cout << "Exited Chat Loop\n";

      //Logo Screen Render
      render_menu(renderer);
    }

    //Logo Screen Render
    // render_menu(renderer);
  }

  // Obviously don't want to do any work if we're quitting
  if(!*quit){
    SDL_Texture* loadScreen = loadFiles(loadImgPath, renderer);
    SDL_RenderCopy(renderer, loadScreen, NULL, NULL);
    SDL_RenderPresent(renderer);
    return seed;
  }
  return seed;
}

void render_menu(SDL_Renderer* renderer){
  save.open("save.txt", std::fstream::in);
  if (save) {
    logoImgPath = "Art/Logo/Continue_Screen.png";
  }
  save.close();
  // Load the title screens
  logoScreen = loadFiles(logoImgPath, renderer);

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, logoScreen, NULL, NULL);
  SDL_RenderPresent(renderer);
}
