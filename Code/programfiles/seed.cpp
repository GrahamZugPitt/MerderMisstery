#include "seed.hpp"
#include "main_helper.hpp"

// The Seed page template
std::string seedPagePath = "Art/Logo/Seed_Page.png";

std::string enter_seed(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  std::cout << "Entered Seed Loop\n";
  //inSeed boolean value
  bool inSeed = true;

  SDL_Texture* seedPageScreen = loadFiles(seedPagePath, renderer);

  //Render seed page background
  SDL_RenderCopy(renderer, seedPageScreen, NULL, NULL);
  SDL_RenderPresent(renderer);

  // Initialize SDL_ttf library
  TTF_Init();

  // Initialize string that holds the user's input
  std::string seedInput = "";

  // Open a font style and sets size
  TTF_Font* fontStyle = TTF_OpenFont("Fonts/Arial.ttf", 60);
  if(!fontStyle) {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
  }

  // Set White variable in rgb format
  SDL_Color textWhite = {255, 255, 255};

  // TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
  SDL_Surface* surfaceInputText = TTF_RenderText_Solid(fontStyle, seedInput.c_str(), textWhite);

  // now you can convert it into a texture
  SDL_Texture* textureInputText = SDL_CreateTextureFromSurface(renderer, surfaceInputText);

  SDL_Rect inputRect; // create a rect

  int textW = 0;
  int textH = 0;

  // returns inherent width and height of font determed by size set in fontStyle variable
  // stores these values in textH and textW variables
  SDL_QueryTexture(textureInputText, NULL, NULL, &textW, &textH);

  inputRect.x = 235;  // controls the rect's x coordinate
  inputRect.y = 593; // controls the rect's y coordinte
  inputRect.w = textW; // controls the width of the rect
  inputRect.h = textH; // controls the height of the rect

  // copy blank input texture to the renderer
  SDL_RenderCopy(renderer, textureInputText, NULL, &inputRect);
  SDL_RenderSetClipRect(renderer, NULL);

  // set render draw color to be the same as the input box color
  // will be used to fill a blank rectangle to cover up old text
  SDL_SetRenderDrawColor(renderer, 91, 71, 4, 0);

  SDL_Rect blankRect; // create a rect to be used to display a blank box
  blankRect.x = 235;  //controls the rect's x coordinate
  blankRect.y = 593; // controls the rect's y coordinte
  blankRect.w = textW; // controls the width of the rect
  blankRect.h = textH; // controls the height of the rect

  // copy blank rect to the renderer, fill it with render draw color, and update
  SDL_RenderDrawRect(renderer, &blankRect);
  SDL_RenderFillRect(renderer, &blankRect);
  SDL_RenderPresent(renderer);

  //Enable text input
  SDL_StartTextInput();

  //Wait for user to exit chat
  while (inSeed == true && !(*quit)) {

  	//The rerender text flag
    bool renderText = false;

  	while (SDL_PollEvent(&e) != 0) {

  	  //Quit application
      if(e.type == SDL_QUIT) {
        *quit = true;
      }
      else if (e.type == SDL_KEYDOWN) {
        //Handle backspace
        if(e.key.keysym.sym == SDLK_BACKSPACE && seedInput.length() > 0) {
          //lop off character
          seedInput.pop_back();
          renderText = true;
        }
        //Handle 'sending' seed by pressing enter or return
        else if(e.key.keysym.sym == SDLK_RETURN) {
          // here is where the 'sending' the seed will happen
          //seedInput=" "; Not sure why this line was here
          renderText = true;
          inSeed=false;

          std::cout << "Seed was entered\n";
        }
        //Handle copy
        else if(e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
          SDL_SetClipboardText(seedInput.c_str());
        }
        //Handle paste
        else if(e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
          seedInput = SDL_GetClipboardText();
          renderText = true;
        }
      }
      //Special text input event
      else if(e.type == SDL_TEXTINPUT) {
        //Not copy or pasting
        if(!( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V'))) {
          //Append character
          seedInput += e.text.text;
          renderText = true;
        }
      }
  	}


  	//Rerender text if needed
    if(renderText) {

      //Text is not empty
      if(seedInput != "") {
        //Render new text. Create surface first and then texture
        surfaceInputText = TTF_RenderText_Solid(fontStyle, seedInput.c_str(), textWhite);
        textureInputText = SDL_CreateTextureFromSurface(renderer, surfaceInputText);
      }
      //Text is empty
      else {
        surfaceInputText = TTF_RenderText_Solid(fontStyle, " ", textWhite);
        textureInputText = SDL_CreateTextureFromSurface(renderer, surfaceInputText);
      }
      SDL_QueryTexture(textureInputText, NULL, NULL, &textW, &textH);
      inputRect.w = textW;
      inputRect.h = textH;
      blankRect.h = textH;
      if(textW>blankRect.w) {
      	blankRect.w = textW; // controls the width of the rect
      }

      SDL_RenderDrawRect(renderer, &blankRect);
      SDL_RenderFillRect(renderer, &blankRect);
      SDL_RenderPresent(renderer);

      //render the textures and update screen
      SDL_RenderCopy(renderer, textureInputText, NULL, &inputRect);
      SDL_RenderSetClipRect(renderer, NULL);
      SDL_RenderPresent(renderer);
    }

    //Quit application
    if(e.type == SDL_QUIT)
        *quit = true;

    // Get the Keyboard State
    keyState = SDL_GetKeyboardState(NULL);

    if (keyState[SDL_SCANCODE_ESCAPE] || inSeed == false)
    {
      inSeed = false;

      //free memory for all text related items that were intialized
      TTF_CloseFont(fontStyle);
      SDL_FreeSurface(surfaceInputText);
      SDL_DestroyTexture(textureInputText);

      //Disable text input
      SDL_StopTextInput();

      // Quit SDL TTF subsystem
      TTF_Quit();

      SDL_RenderClear(renderer);
      //make sure to end things and return seed
      return seedInput;
    }
  }
}
