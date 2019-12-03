#include "info.hpp"

const int ALL_BUTTON_TOP = 650;
const int ALL_BUTTON_BOTTOM = 696;
const int PREV_BUTTON_LEFT = 485;
const int PREV_BUTTON_RIGHT = 580;
const int NEXT_BUTTON_LEFT = 698;
const int NEXT_BUTTON_RIGHT = 792;

// The Information screen templates
const std::string dir = "Art/Information/";
const std::string ext = ".png";
std::vector <std::string> infostrs{"info1", "info2", "info3", "info4"};

bool getInfo;
int currPage = 1;
int cursor_x;
int cursor_y;
std::string name, fullPath;
SDL_Texture* infoImg = nullptr;

void info_page(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer)
{
  getInfo = true;
  // currPage = 1;

  std::cout << "information\n";

  get_page(renderer);

  while(getInfo && !(*quit)) {
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

      // check if click was in Y range of buttons
      // if it wasn't in Y range, no need to check X range
      if((cursor_y < ALL_BUTTON_BOTTOM) && (cursor_y > ALL_BUTTON_TOP))
      {
        // cursor_x value comparisons to button ranges
        if((cursor_x > PREV_BUTTON_LEFT) && (cursor_x < PREV_BUTTON_RIGHT)){
          if(currPage > 1)
          {
            currPage--;
            get_page(renderer);
            break;
          }
        }
        else if((cursor_x > NEXT_BUTTON_LEFT) && (cursor_x < NEXT_BUTTON_RIGHT)){
          if(currPage < 4)
          {
            currPage++;
            get_page(renderer);
            break;
          }
        }
      }
    }

    // Get the Keyboard State
    keyState = SDL_GetKeyboardState(NULL);

    if(keyState[SDL_SCANCODE_LCTRL] || keyState[SDL_SCANCODE_RCTRL]){
      if (keyState[SDL_SCANCODE_E]){
        std::cout << "Exiting info\n";
        getInfo = false;
        currPage = 1;
        SDL_RenderClear(renderer);
        SDL_DestroyTexture(infoImg);
        return;
      }
    }
  } //end while loop
} //end info_page

void get_page(SDL_Renderer* renderer)
{
  //currPage - 1 because page 1 is in infostrs[0]
  name = infostrs[currPage - 1];
  fullPath = dir + name + ext;

  // load image files
  infoImg = loadFiles(fullPath.c_str(), renderer);

  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, infoImg, NULL, NULL);
  SDL_RenderPresent(renderer);

  std::cout << "information page: ";
  std::cout << currPage;
  std::cout << "\n";
}
