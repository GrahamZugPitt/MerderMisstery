#include "chat.hpp"
#include "main_helper.hpp"

// The Chat screen template
std::string chatScreenPath = "Art/Chat/chat_template.png";

void enter_chat(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  std::cout << "Entering Chat\n";
  //inChat boolean value
  bool inChat = true;

  SDL_Texture* chatScreen = loadFiles(chatScreenPath, renderer);

  //Render chat template
  SDL_RenderCopy(renderer, chatScreen, NULL, NULL);

  //User input box left X: 32
  //User input box right X: 1247
  //User input box top y: 597
  //User input box bottom y: 710

  //Initialize SDL_ttf library
  TTF_Init();

  //Open a font style and sets size
  TTF_Font* Text = TTF_OpenFont("Fonts/Arial.ttf", 25);
  if(!Text) {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
  }

  //Set White variable in rgb format
  SDL_Color White = {255, 255, 255};

  //TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
  SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Text, "please show on the screen", White);

  //now you can convert it into a texture
  SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

  SDL_Rect input_rect; //create a rect
  input_rect.x = 32;  //controls the rect's x coordinate
  input_rect.y = 597; // controls the rect's y coordinte
  input_rect.w = 1215; // controls the width of the rect
  input_rect.h = 113; // controls the height of the rect

  SDL_RenderCopy(renderer, Message, NULL, &input_rect);
  SDL_RenderSetClipRect(renderer, NULL);
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
      {
        inChat = false;
        TTF_CloseFont(Text);
        TTF_Quit();
        SDL_DestroyTexture(Message);
        // gWindow = NULL;
        // gRenderer = NULL;
        // gFont = NULL;

        // Quit SDL TTF subsystem
        TTF_Quit();
        //make sure to end things to close client and clear stuff that needs cleared
      }
    }
  }
}
