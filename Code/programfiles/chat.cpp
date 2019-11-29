#include "chat.hpp"
#include "main_helper.hpp"

// The Chat screen template
const std::string chatScreenPath = "Art/Chat/chat_template.png";
//initialize chatScreen texture
SDL_Texture* chatScreen;

//inChat boolean value
bool inChat;

//Set White variable in rgb format
const SDL_Color textWhite = {255, 255, 255};

//intialize TTF_Font variable to be set later
TTF_Font* fontStyle;

//initialize text width and height ints
int textW = 0;
int textH = 0;

//Initialize string that holds the user's input and messages from server
std::string inputText, textFromServer;

//initialize surface and texture for server messages and input text
SDL_Surface* surfaceMessage;
SDL_Surface* surfaceInputText;

SDL_Texture* textureMessage;
SDL_Texture* textureInputText;

//create a rect for the text textures to appear in
SDL_Rect inputRect;
SDL_Rect serverMessageRect;

/*layout dimensions
    User input box left X: 32
    User input box right X: 1247
    User input box top y: 597
    User input box bottom y: 710
*/

void enter_chat(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  std::cout << "Entered Chat Loop\n";

  //set inChat boolean value
  inChat = true;

  //set chatScreen texture
  chatScreen = loadFiles(chatScreenPath, renderer);

  //Initialize SDL_ttf library
  TTF_Init();

  //Open a font style and sets size
  fontStyle = TTF_OpenFont("Fonts/Arial.ttf", 35);
  if(!fontStyle) {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
  }

  //initialize messages from server into serverMessageRect
  update_messages(renderer);

  //initialize user input
  initialize_input(renderer);

  //initialize text from input into inputRect and begin rendering user text
  update_render(renderer);

  // set render draw color to be the same as the input box color
  // will be used to fill a blank rectangle to cover up old text
  SDL_SetRenderDrawColor(renderer, 143, 171, 221, 0);

  SDL_Rect blankRect; // create a rect to be used to display a blank box
  blankRect.x = 36;  //controls the rect's x coordinate
  blankRect.y = 597; // controls the rect's y coordinte
  blankRect.w = textW; // controls the width of the rect
  blankRect.h = textH; // controls the height of the rect
  SDL_RenderDrawRect(renderer, &blankRect);
  SDL_RenderFillRect(renderer, &blankRect);
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
        //Handle 'sending' text by pressing enter or return
        else if(e.key.keysym.sym == SDLK_RETURN) {
          // here is where the 'sending' to the file will happen
          inputText=" ";
          renderText = true;
          std::cout << "Input was 'sent'\n";

        }
        //Handle copy
        else if(e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
          SDL_SetClipboardText(inputText.c_str());
        }
        //Handle paste
        else if(e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
          inputText = SDL_GetClipboardText();
          renderText = true;
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
      SDL_QueryTexture(textureInputText, NULL, NULL, &textW, &textH);
      inputRect.w = textW;
      inputRect.h = textH;
      blankRect.h = textH;
      if(textW>blankRect.w)
        blankRect.w = textW; // controls the width of the rect

      //update messages from server
      update_messages(renderer);
      update_render(renderer);

      SDL_RenderDrawRect(renderer, &blankRect);
      SDL_RenderFillRect(renderer, &blankRect);
      SDL_RenderPresent(renderer);
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
        SDL_FreeSurface(surfaceInputText);
        SDL_DestroyTexture(textureInputText);
        //Disable text input
        SDL_StopTextInput();

        // Quit SDL TTF subsystem
        TTF_Quit();
        //make sure to end things to close client and clear stuff that needs cleared

        SDL_RenderClear(renderer);
        //make sure to end things
        return;
      }
    }
  }
} //end enter_chat

void initialize_input(SDL_Renderer* renderer){
  //set input text surface and convert into texture
  surfaceInputText = TTF_RenderText_Solid(fontStyle, inputText.c_str(), textWhite);
  textureInputText = SDL_CreateTextureFromSurface(renderer, surfaceInputText);

  //returns inherent width and height of font determed by size set in fontStyle variable
  //stores these values in textH and textW variables
  SDL_QueryTexture(textureInputText, NULL, NULL, &textW, &textH);

  //rect for input text
  inputRect.x = 36;  //controls the rect's x coordinate
  inputRect.y = 597; // controls the rect's y coordinte
  inputRect.w = textW; //1215; // controls the width of the rect
  inputRect.h = textH; //113; // controls the height of the rect
} //end initialize_input

void update_messages(SDL_Renderer* renderer){
  //Read in data structure from the server, Colton ;)
  //Need to make sure to append \n (new line) to the end of each message so that
  //it reads in like the example text below

  //take past messages from server and make them a surface --> texture
  textFromServer = "User1: this is from the server kinda but not really "\
   "becuaase i am just typing a bunch of shit to try and see if the text will "\
   "wrap to the next line \nUser2: Thats dope! \nUser3: Rad! \nUser4: The cooliest"\
   "\nUser5: The cooliest \nUser6: The cooliest \nUser7: The cooliest \nUser8: The cooliest";
   /////// THIS ABOVE STRING CAN BE REPLACED BY A STRING READING FILE INPUT FROM THE SERVER////////

  //convert server text into 'message' char needed for TTF_RenderText_Blended_Wrapped
  char message[textFromServer.size() + 1];
	std::copy(textFromServer.begin(), textFromServer.end(), message);
	message[textFromServer.size()] = '\0';

  //set message text surface and convert into texture, 1215 sets the text to
  //wrap to the next line at that pixel
  surfaceMessage = TTF_RenderText_Blended_Wrapped(fontStyle, message, textWhite, 1215);
  textureMessage = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

  //returns inherent width and height of font determed by size set in fontStyle variable
  //stores these values in textH and textW variables
  SDL_QueryTexture(textureMessage, NULL, NULL, &textW, &textH);

  //rect for messages from server
  serverMessageRect.x = 36;
  serverMessageRect.y = 120;
  serverMessageRect.w = textW;
  serverMessageRect.h = textH;

} //end update_messages

// render chatScreen, textureMessage, and textureInputText
void update_render(SDL_Renderer* renderer){
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, chatScreen, NULL, NULL); //Render chat template
  SDL_RenderCopy(renderer, textureMessage, NULL, &serverMessageRect);
  SDL_RenderCopy(renderer, textureInputText, NULL, &inputRect);
  // SDL_RenderSetClipRect(renderer, NULL); dont think we need this
  SDL_RenderPresent(renderer);
} //end update_render
