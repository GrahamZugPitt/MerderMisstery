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
  std::string inputText = "Input Text";

  //Open a font style and sets size
  TTF_Font* fontStyle = TTF_OpenFont("Fonts/Arial.ttf", 25);
  if(!fontStyle) {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
  }

  //Set White variable in rgb format
  SDL_Color textWhite = {255, 255, 255};

  //TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
  SDL_Surface* surfaceInputText = TTF_RenderText_Solid(fontStyle, inputText.c_str(), textWhite);

  //now you can convert it into a texture
  SDL_Texture* textureInputText = SDL_CreateTextureFromSurface(renderer, surfaceInputText);

  SDL_Rect inputRect; //create a rect

  int textW = 0;
  int textH = 0;
  //returns inherent width and height of font determed by size set in fontStyle variable
  //stores these values in textH and textW variables
  SDL_QueryTexture(textureInputText, NULL, NULL, &textW, &textH);

  inputRect.x = 36;  //controls the rect's x coordinate
  inputRect.y = 597; // controls the rect's y coordinte
  inputRect.w = textW; //1215; // controls the width of the rect
  inputRect.h = textH; //113; // controls the height of the rect
  //text will most likely still go outside the input box, need to clean up with
  //example from https://gamedev.stackexchange.com/questions/140294/what-is-the-most-efficient-way-to-render-a-textbox-in-c-sdl2

  SDL_RenderCopy(renderer, textureInputText, NULL, &inputRect);
  SDL_RenderSetClipRect(renderer, NULL);
  SDL_RenderPresent(renderer);

  //Enable text input
  SDL_StartTextInput();

  //Wait for user to exit chat
  while (inChat == true && !(*quit)) {

    //The rerender text flag
    bool renderText = false;

    while (SDL_PollEvent(&e) != 0) {

      //Quit application
      if(e.type == SDL_QUIT) {
        *quit = true;
      }
      else if (e.type == SDL_KEYDOWN) {
        //Handle backspace
        if(e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0) {
          //lop off character
          inputText.pop_back();
          renderText = true;
        }
        //Handle copy
        else if(e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
          SDL_SetClipboardText( inputText.c_str() );
        }
        //Handle paste
        else if(e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
          inputText = SDL_GetClipboardText();
          renderText = true;
        }
        //Handle ctrl e to exit
        else if(e.key.keysym.sym == SDLK_e && SDL_GetModState() & KMOD_CTRL) {

        }
      }
      //Special text input event
      else if(e.type == SDL_TEXTINPUT) {
        //Not copy or pasting
        if(!( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V'))) {
          //Append character
          inputText += e.text.text;
          renderText = true;
        }
      }
    }

    //Rerender text if needed
    if(renderText) {

      //Text is not empty
      if(inputText != "") {
        //Render new text. Create surface first and then texture
        surfaceInputText = TTF_RenderText_Solid(fontStyle, inputText.c_str(), textWhite);
        textureInputText = SDL_CreateTextureFromSurface(renderer, surfaceInputText);
      }
      //Text is empty
      else {
        surfaceInputText = TTF_RenderText_Solid(fontStyle, " ", textWhite);
        textureInputText = SDL_CreateTextureFromSurface(renderer, surfaceInputText);
      }

      // here is where i need to clear the previously rendered text, but I am not sure how to do it

      inputRect.w = textW; 
      inputRect.h = textH;
      SDL_QueryTexture(textureInputText, NULL, NULL, &textW, &textH);
      //render the textures and update screen
      SDL_RenderCopy(renderer, textureInputText, NULL, &inputRect);
      SDL_RenderSetClipRect(renderer, NULL);
      SDL_RenderPresent(renderer);
    }

    // Get the Keyboard State
    keyState = SDL_GetKeyboardState(NULL);

    // Open Chat room
    if (keyState[SDL_SCANCODE_LCTRL] || keyState[SDL_SCANCODE_RCTRL])
    {
      if (keyState[SDL_SCANCODE_E])
      {
        inChat = false;

        //free memory for all text related items that were intialized
        TTF_CloseFont(fontStyle);
        TTF_Quit();
        SDL_DestroyTexture(textureInputText);
        //Disable text input
        SDL_StopTextInput();

        // Quit SDL TTF subsystem
        TTF_Quit();
        //make sure to end things to close client and clear stuff that needs cleared
      }
    }
  }

  
}
