#include "chat.hpp"
#include "main_helper.hpp"

// The Chat example template
std::string chatExamplePath = "Art/chat_example.png";

void enter_chat(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  std::cout << "Entering Chat\n";
  //inChat boolean value
  bool inChat = true;

  SDL_Texture* chatExampleScreen = loadFiles(chatExamplePath, renderer);

  //Render chat coming soon page
  SDL_RenderCopy(renderer, chatExampleScreen, NULL, NULL);
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
    if (keyState[SDL_SCANCODE_LCTRL] || keyState[SDL_SCANCODE_RCTRL])
    {
      if (keyState[SDL_SCANCODE_E])
          inChat = false;
    }
  }
}
