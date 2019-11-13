#include "chat.hpp"
#include "main_helper.hpp"
#include <SDL2/SDL_ttf.h>

std::string discussionBoxPath = "Art/DiscussionImages/DiscussionBox.png";
std::string selectedBoxPath = "Art/DiscussionImages/TextBoxSelected.png";
std::string deselectedBoxPath = "Art/DiscussionImages/TextBoxDeselected.png";

SDL_Rect useless;

int boxw, boxh, width_offset, height_offset, tl_x, tl_y, tr_x, tr_y, bl_x, bl_y, br_x, br_y;

std::string defaultName = "Waldo";

// An initial function to get everything set up
void setup_vars(SDL_Texture *selected){
  SDL_QueryTexture(selected, NULL, NULL, &boxw, &boxh);

  int width_offset = (SCREEN_WIDTH - 2 * boxw) / 3;
  int height_offset = (SCREEN_HEIGHT - h - 2 * boxh) / 3;

  int tl_x = width_offset;
  int tl_y = SCREEN_HEIGHT - h + height_offset;

  int tr_x = tl_x + boxw + width_offset;
  int tr_y = tl_y;

  int bl_x = tl_x;
  int bl_y = tl_y + boxh + height_offset - 25;

  int br_x = tr_x;
  int br_y = bl_y;
}

void draw_boxes(int selectnum, SDL_Renderer *renderer, SDL_Texture *selected, SDL_Texture *deselected, int w, int h){
  if(selectnum != 1)
    renderTexture(renderer, deselected, useless, tl_x, tl_y, boxw, boxh, false);
  if(selectnum != 2)
    renderTexture(renderer, deselected, useless, tr_x, tr_y, boxw, boxh, false);
  if(selectnum != 3)
    renderTexture(renderer, deselected, useless, bl_x, bl_y, boxw, boxh, false);
  if(selectnum != 4)
    renderTexture(renderer, deselected, useless, br_x, br_y, boxw, boxh, false);

  int selected_x, selected_y;

  switch(selectnum){
    case 1:
      selected_x = tl_x;
      selected_y = tl_y;
      break;
    case 2:
      selected_x = tr_x;
      selected_y = tr_y;
      break;
    case 3:
      selected_x = bl_x;
      selected_y = bl_y;
      break;
    default: // Only other possible case is 4 so this is an else
      selected_x = br_x;
      selected_y = br_y;
      break;
  }

  renderTexture(renderer, selected, useless, selected_x, selected_y, boxw, boxh, false);
}

// The function to actually draw the text on the screen
void draw_text(renderer){
  
}

void enter_discussion(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  SDL_Texture* discussionBoxTex = loadFiles(discussionBoxPath, renderer);
  SDL_Texture* selectedBoxTex = loadFiles(selectedBoxPath, renderer);
  SDL_Texture* deselectedBoxTex = loadFiles(deselectedBoxPath, renderer);

  setup_vars(selectedBoxTex);

  TTF_Font *TNR = TTF_OpenFont("Art/Font/times-new-roman.ttf", 24); // Opens the font and sets the size

  SDL_Color White = {255, 255, 255};

  SDL_Surface *surfaceMessage = TTF_Render_Text_Solid(TNR, "Hello world", White);

  SDL_Texture *TLBox = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
  SDL_Rect TLBoxRect;
  TLBoxRect.x =

  int selected = 1;

  //Clear screen
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
  SDL_RenderClear(renderer);

  bool inDiscussion = true;

  //Wait for user to exit chat
  while (SDL_PollEvent(&e) != 0 || inDiscussion)
  {
    //Quit application
    if(e.type == SDL_QUIT){
        *quit = true;
        return;
    }

    // Input Handler
    // Get the Keyboard State
    keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_Q])
        return;

    if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]){
      if(selected == 3) selected = 1;
      if(selected == 4) selected = 2;
    }

    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]){
      if(selected == 2) selected = 1;
      if(selected == 4) selected = 3;
    }

    if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN]){
      if(selected == 1) selected = 3;
      if(selected == 2) selected = 4;
    }

    if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]){
      if(selected == 1) selected = 2;
      if(selected == 3) selected = 4;
    }

    // Draw Handler
    int w, h;
    SDL_QueryTexture(discussionBoxTex, NULL, NULL, &w, &h);
    renderTexture(renderer, discussionBoxTex, useless, 0, SCREEN_HEIGHT - h, w, h, false);

    draw_boxes(selected, renderer, selectedBoxTex, deselectedBoxTex, w, h);
    draw_text(renderer);

    //Update Screen
    SDL_RenderPresent(renderer);
  }
}
