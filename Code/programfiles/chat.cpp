#include "chat.hpp"
#include "main_helper.hpp"

// The "Chat coming soon picture"
std::string chatSoonPath = "Art/chat_soon.png";

void enter_chat(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  std::cout << "Entering Chat\n";
  //inChat boolean value
  bool inChat = true;

  SDL_Texture* chatSoonScreen = loadFiles(chatSoonPath, renderer);

  //Render chat coming soon page
  SDL_RenderCopy(renderer, chatSoonScreen, NULL, NULL);
  SDL_RenderPresent(renderer);

  //Wait for user to exit chat
  while (SDL_PollEvent(&e) != 0 || (inChat == true && !(*quit)))
  {
    //Quit application
    if(e.type == SDL_QUIT)
        *quit = true;

    // Get the Keyboard State
    keyState = SDL_GetKeyboardState(NULL);

    // Open Chat room
    if (keyState[SDL_SCANCODE_E])
        inChat = false;
  }
}
