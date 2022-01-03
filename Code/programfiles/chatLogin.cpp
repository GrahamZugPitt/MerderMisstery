#include "chatLogin.hpp"
#include "main_helper.hpp"

using namespace std;

// The Chat screen template
const std::string chatLoginPath = "Art/Chat/chat_login.png";
//initialize chatScreen texture
SDL_Texture* chatLoginScreen;

// The Chat error template
const std::string chatErrorPath = "Art/Chat/chat_error.png";
//initialize chatError texture
SDL_Texture* chatError;

//inLogin boolean value
bool inLogin;

//Set White variable in rgb format
const SDL_Color textWhite = {255, 255, 255};

//intialize TTF_Font variable to be set later
TTF_Font* fontStyle_login;

//initialize text width and height ints
int textW_login = 0;
int textH_login = 0;

// 3 strings were initialized as global variables to use for this: userText, passText, hostText

//Initialize strings that hold the username, password, and hostname for chat
std::string userText, passText, hostText;

//initialize surface and texture for input text for all 3 fields
SDL_Surface* surfaceInputUser;
SDL_Surface* surfaceInputPass;
SDL_Surface* surfaceInputHost;

SDL_Texture* textureInputUser;
SDL_Texture* textureInputPass;
SDL_Texture* textureInputHost;

//create a rect for the text textures to appear in
SDL_Rect userRect;
SDL_Rect passRect;
SDL_Rect hostRect;

/*layout dimensions
    
*/

vector<string> enter_login(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer){
  std::cout << "Entered Chat Login Loop\n";

  vector<string> credentials;

  //set inLogin boolean value
  inLogin = true;

  //set chatScreen texture
  chatLoginScreen = loadFiles(chatLoginPath, renderer);

  //Initialize SDL_ttf library
  TTF_Init();

  //Open a font style and sets size
  fontStyle_login = TTF_OpenFont("Fonts/Arial.ttf", 35);
  if(!fontStyle_login) {
      printf("TTF_OpenFont: %s\n", TTF_GetError());
  }

  //initialize user input
  initialize_input_login(renderer);

  //initialize text from input into inputRect and begin rendering user text
  update_render_login(renderer);

  // set render draw color to be the same as the input box color
  // will be used to fill a blank rectangle to cover up old text
  SDL_SetRenderDrawColor(renderer, 143, 171, 221, 0);

  SDL_Rect blankRect1; // create a rect to be used to display a blank box
  blankRect1.x = 36;  //controls the rect's x coordinate
  blankRect1.y = 190; // controls the rect's y coordinte
  blankRect1.w = textW_login; // controls the width of the rect
  blankRect1.h = textH_login; // controls the height of the rect
  SDL_RenderDrawRect(renderer, &blankRect1);
  SDL_RenderFillRect(renderer, &blankRect1);
  SDL_RenderPresent(renderer);

  SDL_Rect blankRect2; 
  blankRect2.x = 36;  
  blankRect2.y = 325; 
  blankRect2.w = textW_login; 
  blankRect2.h = textH_login; 
  SDL_RenderDrawRect(renderer, &blankRect2);
  SDL_RenderFillRect(renderer, &blankRect2);
  SDL_RenderPresent(renderer);

  SDL_Rect blankRect3; // create a rect to be used to display a blank box
  blankRect3.x = 36;  //controls the rect's x coordinate
  blankRect3.y = 470; // controls the rect's y coordinte
  blankRect3.w = textW_login; // controls the width of the rect
  blankRect3.h = textH_login; // controls the height of the rect
  SDL_RenderDrawRect(renderer, &blankRect3);
  SDL_RenderFillRect(renderer, &blankRect3);
  SDL_RenderPresent(renderer);

  //Enable text input
  SDL_StartTextInput();

  int pressedEnter = 0;
  //Wait for user to exit chat
  while (inLogin == true && !(*quit) && pressedEnter<3) {

    //The rerender text flag
    bool renderUserText = false;
    bool renderPassText = false;
    bool renderHostText = false;

    while (SDL_PollEvent(&e) != 0) {

      //Quit application
      if(e.type == SDL_QUIT) {
        *quit = true;
      }
      else if (e.type == SDL_KEYDOWN) {
        //Handle backspace
        if(e.key.keysym.sym == SDLK_BACKSPACE && pressedEnter==0 && userText.length() > 0) {
          //lop off character
          userText.pop_back();
          renderUserText = true;
        }
        else if(e.key.keysym.sym == SDLK_BACKSPACE && pressedEnter==1 && passText.length() > 0) {
          //lop off character
          passText.pop_back();
          renderPassText = true;
        }
        else if(e.key.keysym.sym == SDLK_BACKSPACE && pressedEnter==2 && hostText.length() > 0) {
          //lop off character
          hostText.pop_back();
          renderHostText = true;
        }
        //Handle 'sending' text by pressing enter or return
        else if(e.key.keysym.sym == SDLK_RETURN) {
          // here is where the 'sending' to the file will happen
          if(pressedEnter==0) {
            std::cout << "Username entered\n";
            renderUserText=true;
          }
          else if (pressedEnter==1) {
            std::cout << "Password entered\n";
            renderPassText=true;
          }
          else if (pressedEnter==2) {
            std::cout << "Hostname entered\n";
            renderHostText=true;
          }

          pressedEnter++;
          //std::cout << "Input was 'sent': " << text << "\n";
        }
        
      }
      //Special text input event
      else if(e.type == SDL_TEXTINPUT) {
        //Not copy or pasting
        if(!( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V'))) {
          //Append character
          if(pressedEnter==0) {
            userText += e.text.text;
            renderUserText=true;
          }
          else if (pressedEnter==1) {
            passText += e.text.text;
            renderPassText=true;
          }
          else if (pressedEnter==2) {
            hostText += e.text.text;
            renderHostText=true;
          }
        }
      }
    }


    //----------------------------------------Rerender user text if needed
    if(renderUserText) {

      //Text is not empty
      if(userText != "") {
        //Render new text. Create surface first and then texture
        surfaceInputUser = TTF_RenderText_Blended_Wrapped(fontStyle_login, userText.c_str(), textWhite, 1215);
        textureInputUser = SDL_CreateTextureFromSurface(renderer, surfaceInputUser);
      }
      //Text is empty
      else {
        surfaceInputUser = TTF_RenderText_Blended_Wrapped(fontStyle_login, " ", textWhite, 1215);
        textureInputUser = SDL_CreateTextureFromSurface(renderer, surfaceInputUser);
      }
      SDL_QueryTexture(textureInputUser, NULL, NULL, &textW_login, &textH_login);
      userRect.w = textW_login;
      userRect.h = textH_login;
      blankRect1.h = textH_login;
      if(textW_login>blankRect1.w)
        blankRect1.w = textW_login; // controls the width of the rect

      update_render_login(renderer);
      
      SDL_RenderDrawRect(renderer, &blankRect1);
      SDL_RenderFillRect(renderer, &blankRect1);
      SDL_RenderPresent(renderer);
      //render the textures and update screen
      SDL_RenderCopy(renderer, textureInputUser, NULL, &userRect);
      SDL_RenderSetClipRect(renderer, NULL);
      SDL_RenderPresent(renderer);
    }




    //----------------------------------------Rerender pass text if needed
    if(renderPassText) {

      //Text is not empty
      if(passText != "") {
        //Render new text. Create surface first and then texture
        surfaceInputPass = TTF_RenderText_Blended_Wrapped(fontStyle_login, passText.c_str(), textWhite, 1215);
        textureInputPass = SDL_CreateTextureFromSurface(renderer, surfaceInputPass);
      }
      //Text is empty
      else {
        surfaceInputPass = TTF_RenderText_Blended_Wrapped(fontStyle_login, " ", textWhite, 1215);
        textureInputPass = SDL_CreateTextureFromSurface(renderer, surfaceInputPass);
      }
      SDL_QueryTexture(textureInputPass, NULL, NULL, &textW_login, &textH_login);
      passRect.w = textW_login;
      passRect.h = textH_login;
      blankRect2.h = textH_login;
      if(textW_login>blankRect2.w)
        blankRect2.w = textW_login; // controls the width of the rect

      update_render_login(renderer);
      
      SDL_RenderDrawRect(renderer, &blankRect2);
      SDL_RenderFillRect(renderer, &blankRect2);
      SDL_RenderPresent(renderer);
      //render the textures and update screen
      SDL_RenderCopy(renderer, textureInputPass, NULL, &passRect);
      SDL_RenderSetClipRect(renderer, NULL);
      SDL_RenderPresent(renderer);
    }




    //----------------------------------------Rerender host text if needed
    if(renderHostText) {

      //Text is not empty
      if(hostText != "") {
        //Render new text. Create surface first and then texture
        surfaceInputHost = TTF_RenderText_Blended_Wrapped(fontStyle_login, hostText.c_str(), textWhite, 1215);
        textureInputHost = SDL_CreateTextureFromSurface(renderer, surfaceInputHost);
      }
      //Text is empty
      else {
        surfaceInputHost = TTF_RenderText_Blended_Wrapped(fontStyle_login, " ", textWhite, 1215);
        textureInputHost = SDL_CreateTextureFromSurface(renderer, surfaceInputHost);
      }
      SDL_QueryTexture(textureInputHost, NULL, NULL, &textW_login, &textH_login);
      hostRect.w = textW_login;
      hostRect.h = textH_login;
      hostRect.h = textH_login;
      if(textW_login>blankRect3.w)
        blankRect3.w = textW_login; // controls the width of the rect

      update_render_login(renderer);
      
      SDL_RenderDrawRect(renderer, &blankRect3);
      SDL_RenderFillRect(renderer, &blankRect3);
      SDL_RenderPresent(renderer);
      //render the textures and update screen
      SDL_RenderCopy(renderer, textureInputHost, NULL, &hostRect);
      SDL_RenderSetClipRect(renderer, NULL);
      SDL_RenderPresent(renderer);
    }

    // Close login screen
    if (pressedEnter==3) {
      inLogin = false;

      //free memory for all text related items that were intialized
      TTF_CloseFont(fontStyle_login);
      SDL_FreeSurface(surfaceInputUser);
      SDL_FreeSurface(surfaceInputPass);
      SDL_FreeSurface(surfaceInputHost);
      SDL_DestroyTexture(textureInputUser);
      SDL_DestroyTexture(textureInputPass);
      SDL_DestroyTexture(textureInputHost);
      //Disable text input
      SDL_StopTextInput();

      // Quit SDL TTF subsystem
      TTF_Quit();
      //make sure to end things to close client and clear stuff that needs cleared

      SDL_RenderClear(renderer);
      //make sure to end things
      credentials.push_back(userText);
      credentials.push_back(passText);
      credentials.push_back(hostText);
      return credentials;
    }
  }
} //end enter_chat


void initialize_input_login(SDL_Renderer* renderer){
  //set input text surface and convert into texture
  surfaceInputUser = TTF_RenderText_Blended_Wrapped(fontStyle_login, userText.c_str(), textWhite, 1215);
  surfaceInputPass = TTF_RenderText_Blended_Wrapped(fontStyle_login, passText.c_str(), textWhite, 1215);
  surfaceInputHost = TTF_RenderText_Blended_Wrapped(fontStyle_login, hostText.c_str(), textWhite, 1215);
  textureInputUser = SDL_CreateTextureFromSurface(renderer, surfaceInputUser);
  textureInputPass = SDL_CreateTextureFromSurface(renderer, surfaceInputPass);
  textureInputHost = SDL_CreateTextureFromSurface(renderer, surfaceInputHost);

  //returns inherent width and height of font determed by size set in fontStyle_login variable
  //stores these values in textH_login and textW_login variables
  SDL_QueryTexture(textureInputUser, NULL, NULL, &textW_login, &textH_login);

  //rect for input text
  userRect.x = 36;  //controls the rect's x coordinate
  userRect.y = 190; // controls the rect's y coordinte
  userRect.w = textW_login; //1215; // controls the width of the rect
  userRect.h = textH_login; //113; // controls the height of the rect

  SDL_QueryTexture(textureInputPass, NULL, NULL, &textW_login, &textH_login);

  //rect for input text
  passRect.x = 36;  //controls the rect's x coordinate
  passRect.y = 325; // controls the rect's y coordinte
  passRect.w = textW_login; //1215; // controls the width of the rect
  passRect.h = textH_login; //113; // controls the height of the rect

  SDL_QueryTexture(textureInputHost, NULL, NULL, &textW_login, &textH_login);

  //rect for input text
  hostRect.x = 36;  //controls the rect's x coordinate
  hostRect.y = 470; // controls the rect's y coordinte
  hostRect.w = textW_login; //1215; // controls the width of the rect
  hostRect.h = textH_login; //113; // controls the height of the rect
} //end initialize_input

// render chatScreen, textureMessage, and textureInputText
void update_render_login(SDL_Renderer* renderer){
  SDL_RenderClear(renderer);
  SDL_RenderCopy(renderer, chatLoginScreen, NULL, NULL); //Render chat login template
  SDL_RenderCopy(renderer, textureInputUser, NULL, &userRect);
  SDL_RenderCopy(renderer, textureInputPass, NULL, &passRect);
  SDL_RenderCopy(renderer, textureInputHost, NULL, &hostRect);
  // SDL_RenderSetClipRect(renderer, NULL); dont think we need this
  SDL_RenderPresent(renderer);
} //end update_render

