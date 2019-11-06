#include "chat.hpp"
#include "main_helper.hpp"

// The Chat example template
std::string discussionBoxPath = "Art/DiscussionImages/DiscussionBox.png";

void enter_discussion(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  SDL_Texture* discussionBoxTex = loadFiles(discussionBoxPath, renderer);
  SDL_Rect useless;

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

    //Open Chat room
    if (keyState[SDL_SCANCODE_Q])
        return;

    // Draw Handler
    // 977 is the distance from the top of the screen to the top of the texture
    //    assuming the texture touches the bottom of the screen
    int w, h;
    SDL_QueryTexture(discussionBoxTex, NULL, NULL, &w, &h);
    renderTexture(renderer, discussionBoxTex, useless, 0, SCREEN_HEIGHT - h, w, h, false);

    //Update Screen
    SDL_RenderPresent(renderer);
  }
}
