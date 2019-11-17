#include "chat.hpp"
#include "main_helper.hpp"

std::string discussionBoxPath = "Art/DiscussionImages/DiscussionBox.png";
std::string selectedBoxPath = "Art/DiscussionImages/TextBoxSelected.png";
std::string deselectedBoxPath = "Art/DiscussionImages/TextBoxDeselected.png";

SDL_Rect useless;

int boxw, boxh, width_offset, height_offset, tl_x, tl_y, tr_x, tr_y, bl_x, bl_y, br_x, br_y, w, h, selected_x, selected_y;

char *TLString = "Who do you think killed Butler?";
char *TRString = "What is your last memory of the victim?";
char *BLString = "How does Tom feel about Jerry?";
char *BRString = "Where were you last evening?";

// An initial function to get everything set up
void setup_vars(SDL_Texture *selected, SDL_Texture *discussionBoxTex){
  SDL_QueryTexture(selected, NULL, NULL, &boxw, &boxh);
  SDL_QueryTexture(discussionBoxTex, NULL, NULL, &w, &h);

  width_offset = (SCREEN_WIDTH - 2 * boxw) / 3;
  height_offset = (SCREEN_HEIGHT - h - 2 * boxh) / 3;

  tl_x = width_offset;
  tl_y = SCREEN_HEIGHT - h + height_offset;

  tr_x = tl_x + boxw + width_offset;
  tr_y = tl_y;

  bl_x = tl_x;
  bl_y = tl_y + boxh + height_offset - 25;

  br_x = tr_x;
  br_y = bl_y;
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
void draw_text(SDL_Renderer *renderer, SDL_Texture *TLBox, SDL_Rect TLBoxRect, SDL_Texture *TRBox, SDL_Rect TRBoxRect, SDL_Texture *BLBox, SDL_Rect BLBoxRect, SDL_Texture *BRBox, SDL_Rect BRBoxRect){
  SDL_RenderCopy(renderer, TLBox, NULL, &TLBoxRect);
  SDL_RenderCopy(renderer, TRBox, NULL, &TRBoxRect);
  SDL_RenderCopy(renderer, BLBox, NULL, &BLBoxRect);
  SDL_RenderCopy(renderer, BRBox, NULL, &BRBoxRect);
}

void enter_discussion(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  SDL_Texture* discussionBoxTex = loadFiles(discussionBoxPath, renderer);
  SDL_Texture* selectedBoxTex = loadFiles(selectedBoxPath, renderer);
  SDL_Texture* deselectedBoxTex = loadFiles(deselectedBoxPath, renderer);

  setup_vars(selectedBoxTex, discussionBoxTex);

  TTF_Font *TNR = TTF_OpenFont("Art/Font/times-new-roman.ttf", 24); // Opens the font and sets the size
  if(!TNR)
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  SDL_Color White = {255, 255, 255};
  // Load the phrases
  SDL_Surface *TLsurfaceMessage = TTF_RenderText_Solid(TNR, TLString, White);
  SDL_Surface *TRsurfaceMessage = TTF_RenderText_Solid(TNR, TRString, White);
  SDL_Surface *BLsurfaceMessage = TTF_RenderText_Solid(TNR, BLString, White);
  SDL_Surface *BRsurfaceMessage = TTF_RenderText_Solid(TNR, BRString, White);
  // Turn them into textures
  SDL_Texture *TLBox = SDL_CreateTextureFromSurface(renderer, TLsurfaceMessage);
  SDL_Texture *TRBox = SDL_CreateTextureFromSurface(renderer, TRsurfaceMessage);
  SDL_Texture *BLBox = SDL_CreateTextureFromSurface(renderer, BLsurfaceMessage);
  SDL_Texture *BRBox = SDL_CreateTextureFromSurface(renderer, BRsurfaceMessage);
  // Create the position rectangles
  SDL_Rect TLBoxRect = {tl_x + 10, tl_y, boxw - 20, boxh};
  SDL_Rect TRBoxRect = {tr_x + 10, tr_y, boxw - 20, boxh};
  SDL_Rect BLBoxRect = {bl_x + 10, bl_y, boxw - 20, boxh};
  SDL_Rect BRBoxRect = {br_x + 10, br_y, boxw - 20, boxh};


  int selected = 1;

  //Clear screen
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

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

    renderTexture(renderer, discussionBoxTex, useless, 0, SCREEN_HEIGHT - h, w, h, false);

    SDL_RenderClear(renderer);
    draw_boxes(selected, renderer, selectedBoxTex, deselectedBoxTex, w, h);
    draw_text(renderer, TLBox, TLBoxRect, TRBox, TRBoxRect, BLBox, BLBoxRect, BRBox, BRBoxRect);

    //Update Screen
    SDL_RenderPresent(renderer);
  }
}
