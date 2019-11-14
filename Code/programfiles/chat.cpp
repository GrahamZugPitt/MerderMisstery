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

  int textW = 0;
  int textH = 0;
  //returns inherent width and height of font determed by size set in Text variable
  //stores these values in textH and textW variables
  SDL_QueryTexture(Message, NULL, NULL, &textW, &textH);

  input_rect.x = 36;  //controls the rect's x coordinate
  input_rect.y = 597; // controls the rect's y coordinte
  input_rect.w = textW; //1215; // controls the width of the rect
  input_rect.h = textH; //113; // controls the height of the rect
  //text will most likely still go outside the input box, need to clean up with
  //example from https://gamedev.stackexchange.com/questions/140294/what-is-the-most-efficient-way-to-render-a-textbox-in-c-sdl2

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

        //free memory for all text related items that were intialized
        TTF_CloseFont(Text);
        TTF_Quit();
        SDL_DestroyTexture(Message);

        // Quit SDL TTF subsystem
        TTF_Quit();
        //make sure to end things to close client and clear stuff that needs cleared
      }
    }
  }
}
