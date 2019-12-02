#include "discussion.hpp"
#include "main_helper.hpp"

std::string discussionBoxPath = "Art/DiscussionImages/DiscussionBox.png";
std::string selectedBoxPath = "Art/DiscussionImages/TextBoxSelected.png";
std::string deselectedBoxPath = "Art/DiscussionImages/TextBoxDeselected.png";
std::string singlePlayerPathDisc = "Art/Player/SinglePlayer.png";

SDL_Rect useless;

int nameboxw = 100;
int nameboxh = 50;

int boxw, boxh, width_offset, height_offset, tl_x, tl_y, tr_x, tr_y, bl_x, bl_y, br_x, br_y, w, h, selected_x, selected_y;

std::string TLString = "Who do you think killed Butler?";
std::string TRString = "What is your last memory of the victim?";
std::string BLString = "How does Tom feel about Jerry?";
std::string BRString = "Where were you last evening?";

std::string TLStringGhost = "I believe Finley killed you.";
std::string TRStringGhost = "I believe Skyler killed you.";
std::string BLStringGhost = "I believe Salem killed you.";
std::string BRStringGhost = "I believe Dystentary killed you.";


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

void draw_peoples(SDL_Renderer *renderer, SDL_Texture *playerTex, SDL_Texture *npcTex){
  renderTexture(renderer, playerTex, useless, 277, 36, 224, 288, false);

  SDL_Rect crop, pos;

  crop.x = 12;
	crop.y = 12;
	crop.w = 56;
	crop.h = 72;

  pos.x = 779;
	pos.y = 36;
	pos.w = 224;
	pos.h = 288;

  SDL_RenderCopy(renderer, npcTex, &crop, &pos);
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

int enter_discussion(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, NPC *theNPC){
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
  SDL_Texture* discussionBoxTex = loadFiles(discussionBoxPath, renderer);
  SDL_Texture* selectedBoxTex = loadFiles(selectedBoxPath, renderer);
  SDL_Texture* deselectedBoxTex = loadFiles(deselectedBoxPath, renderer);
  SDL_Texture* playerTex = loadFiles(singlePlayerPathDisc, renderer);
  SDL_Texture* npcTex = theNPC->texture;

  setup_vars(selectedBoxTex, discussionBoxTex);

  TTF_Font *TNR = TTF_OpenFont("Art/Font/times-new-roman.ttf", 24); // Opens the font and sets the size
  if(!TNR)
    printf("TTF_OpenFont: %s\n", TTF_GetError());
  SDL_Color White = {255, 255, 255};
  SDL_Color Yellow = {255, 255, 0};

  SDL_Surface *TLsurfaceMessage, *TRsurfaceMessage, *BLsurfaceMessage, *BRsurfaceMessage, *CharNameSurface;

  // Load the phrases
  if(!(theNPC->isGhost)){
    TLsurfaceMessage = TTF_RenderText_Solid(TNR, TLString.c_str(), White);
    TRsurfaceMessage = TTF_RenderText_Solid(TNR, TRString.c_str(), White);
    BLsurfaceMessage = TTF_RenderText_Solid(TNR, BLString.c_str(), White);
    BRsurfaceMessage = TTF_RenderText_Solid(TNR, BRString.c_str(), White);
  }
  else{
    TLsurfaceMessage = TTF_RenderText_Solid(TNR, TLStringGhost.c_str(), White);
    TRsurfaceMessage = TTF_RenderText_Solid(TNR, TRStringGhost.c_str(), White);
    BLsurfaceMessage = TTF_RenderText_Solid(TNR, BLStringGhost.c_str(), White);
    BRsurfaceMessage = TTF_RenderText_Solid(TNR, BRStringGhost.c_str(), White);
  }
  // Also load the name
  CharNameSurface = TTF_RenderText_Solid(TNR, theNPC->getName().c_str(), Yellow);

  // Turn them into textures
  SDL_Texture *TLBox = SDL_CreateTextureFromSurface(renderer, TLsurfaceMessage);
  SDL_Texture *TRBox = SDL_CreateTextureFromSurface(renderer, TRsurfaceMessage);
  SDL_Texture *BLBox = SDL_CreateTextureFromSurface(renderer, BLsurfaceMessage);
  SDL_Texture *BRBox = SDL_CreateTextureFromSurface(renderer, BRsurfaceMessage);
  SDL_Texture *CharTex = SDL_CreateTextureFromSurface(renderer, CharNameSurface);
  // Create the position rectangles
  SDL_Rect TLBoxRect = {tl_x + 10, tl_y, boxw - 20, boxh};
  SDL_Rect TRBoxRect = {tr_x + 10, tr_y, boxw - 20, boxh};
  SDL_Rect BLBoxRect = {bl_x + 10, bl_y, boxw - 20, boxh};
  SDL_Rect BRBoxRect = {br_x + 10, br_y, boxw - 20, boxh};
  SDL_Rect NameBoxRect = {880 - (nameboxw / 2), 300, nameboxw, nameboxh};

  int selected = 1;

  //Clear screen
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

  bool inDiscussion = true;

  //Wait for user to exit chat
  while (SDL_PollEvent(&e) != 0 || inDiscussion)
  {
    int hasSolved = 0;
    //Quit application
    if(e.type == SDL_QUIT){
        *quit = true;
        return 0;
    }

    // Input Handler
    // Get the Keyboard State
    keyState = SDL_GetKeyboardState(NULL);

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

    if (keyState[SDL_SCANCODE_SPACE] && theNPC->isGhost){
      runWinScreen(e, quit, keyState, renderer);
      hasSolved = 1;
    }

    if (keyState[SDL_SCANCODE_Q] || *quit){
      SDL_DestroyTexture(discussionBoxTex);
      SDL_DestroyTexture(selectedBoxTex);
      SDL_DestroyTexture(deselectedBoxTex);
      SDL_DestroyTexture(playerTex);
      SDL_DestroyTexture(TLBox);
      SDL_DestroyTexture(TRBox);
      SDL_DestroyTexture(BLBox);
      SDL_DestroyTexture(BRBox);
      SDL_DestroyTexture(CharTex);
      return hasSolved;
    }

    SDL_RenderClear(renderer);

    renderTexture(renderer, discussionBoxTex, useless, 0, SCREEN_HEIGHT - h, w, h, false);
    draw_peoples(renderer, playerTex, npcTex);
    draw_boxes(selected, renderer, selectedBoxTex, deselectedBoxTex, w, h);
    draw_text(renderer, TLBox, TLBoxRect, TRBox, TRBoxRect, BLBox, BLBoxRect, BRBox, BRBoxRect);
    SDL_RenderCopy(renderer, CharTex, NULL, &NameBoxRect);

    //Update Screen
    SDL_RenderPresent(renderer);
  }
  return 0;
}
