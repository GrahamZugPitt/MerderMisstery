#include "chat.hpp"
#include "main_helper.hpp"
#include "Users.h"

using namespace std;

// The Chat and friends screen template
const std::string chatScreenPath = "Art/Chat/chat_template.png";
const std::string friendScreenPath = "Art/Chat/friends.png";

//initialize chatScreen and friendScreen texture
SDL_Texture* chatScreen;
SDL_Texture* friendScreen;

//inChat and inFriends boolean value
bool inChat;
bool inFriends;

//Set White variable in rgb format
const SDL_Color textWhite = {255, 255, 255};

//intialize TTF_Font variable to be set later
TTF_Font* fontStyle;

//initialize text width and height ints
int textW = 0;
int textH = 0;

//Initialize string that holds the user's input and messages from server
std::string inputText, textFromServer, friendsFromServer;

//initialize surface and texture for server messages and input text
SDL_Surface* surfaceMessage;
SDL_Surface* surfaceInputText;
SDL_Surface* surfaceFriends;

SDL_Texture* textureMessage;
SDL_Texture* textureInputText;
SDL_Texture* textureFriends;

//create a rect for the text textures to appear in
SDL_Rect inputRect;
SDL_Rect serverMessageRect;
SDL_Rect friendsRect;

//wifi symbol dimensions
const int WIFI_TOP = 32;
const int WIFI_BOTTOM = 95;
const int WIFI_LEFT = 812;
const int WIFI_RIGHT = 902;

/*layout dimensions
    User input box left X: 32
    User input box right X: 1247
    User input box top y: 597
    User input box bottom y: 710
*/

void enter_chat(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, vector<string> credentials){
  std::cout << "Entered Chat Loop\n";

  //set inChat boolean value
  inChat = true;

  int cursor_x;
  int cursor_y;

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
  update_messages(renderer, "");

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

  //Adding Client Code Initialization
  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  struct timeval tv;
  int rv;
  //char s[INET6_ADDRSTRLEN];
  fd_set master;
  fd_set temp;

  tv.tv_sec = 0;

  FD_ZERO(&master);
  FD_ZERO(&temp);
  //FD_SET(sockfd, &master);

  User this_user;

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  //This will be subject to change with the username password thing hostname page
  //First string is the host name and second is the port number
  if ((rv = getaddrinfo(credentials[2].c_str(), "9034", &hints, &servinfo)) != 0){
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return;
  }

  for(p = servinfo; p != NULL; p = p->ai_next){
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("client: socket");
      continue;
    }
    if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1){
      close(sockfd);
      perror("client connect");
      continue;
    }
    break;
  }

  if(p == NULL){
    fprintf(stderr, "client: failed to connect\n");
    return;
  }

  freeaddrinfo(servinfo);

  //username and password check
  string username = credentials[0];
  string password = credentials[1];

  //add something that makes the error page show up here/////////////////////////////////////////////////////////////
  while(true){
    //find the username in the database and make sure it's valid
    this_user = this_user.find_user(username);
    if(this_user.get_username().compare(username) != 0){
      cout << "\nSorry, this username does not exist\n";
      continue;
    }

    if(this_user.check_password(username, password) == false){
      cout << "\nSorry, your password is incorrect\n";
      continue;
    }
    else{
      break;
    }
  }

  //info to send and receive data
  char buf[4096];
  FD_SET(sockfd, &master);

  string text;

  //Wait for user to exit chat
  while (inChat == true && !(*quit)) {

    //The rerender text flag
    bool renderText = false;
    bool send_text = false;

    while (SDL_PollEvent(&e) != 0) {

      // if mouse is clicked and released
      if(e.type == SDL_MOUSEBUTTONUP) {
        // Get mouse position
        cursor_x = e.motion.x;
        cursor_y = e.motion.y;

        // ALL_BUTTON_TOP Y value: 532
        // ALL_BUTTON_BOTTOM Y value: 618
        // check if click was in Y range of buttons
        // if it wasn't in Y range, no need to check X range
        if((cursor_y < WIFI_BOTTOM) && (cursor_y > WIFI_TOP) &&
        (cursor_x > WIFI_LEFT) && (cursor_x < WIFI_RIGHT))
        {
          //set friendScreen texture
          friendScreen = loadFiles(friendScreenPath, renderer);

          //render friend screen
          // SDL_RenderClear(renderer);
          SDL_RenderCopy(renderer, friendScreen, NULL, NULL);




          //set friendsFromServer string to be friends
          friendsFromServer = "Your Friend List: " + this_user.get_friends().stringList();
          /////////^^^^^^^^REPLACE THIS ONE WITH INPUT FROMS ERVER^^^^^^^///////




          //convert server text into 'friends' char needed for TTF_RenderText_Blended_Wrapped
          char friends[friendsFromServer.size() + 1];
          std::copy(friendsFromServer.begin(), friendsFromServer.end(), friends);
          friends[friendsFromServer.size()] = '\0';

          //set friends text surface and convert into texture, 1000 sets the text to
          //wrap to the next line at that pixel
          surfaceFriends = TTF_RenderText_Blended_Wrapped(fontStyle, friends, textWhite, 1000);
          textureFriends = SDL_CreateTextureFromSurface(renderer, surfaceFriends);

          //returns inherent width and height of font determed by size set in fontStyle variable
          //stores these values in textH and textW variables
          SDL_QueryTexture(textureFriends, NULL, NULL, &textW, &textH);

          //rect for messages from server
          friendsRect.x = 300;
          friendsRect.y = 270;
          friendsRect.w = textW;
          friendsRect.h = textH;

          SDL_RenderCopy(renderer, textureFriends, NULL, &friendsRect);
          SDL_RenderPresent(renderer);

          // Get the Keyboard State
          keyState = SDL_GetKeyboardState(NULL);

          while(inFriends && !(*quit)){
            while(SDL_PollEvent(&e) != 0) {
              if(e.type == SDL_QUIT) {
                (*quit) = true;
              }
            }

            if(keyState[SDL_SCANCODE_E]){
              std::cout << "Exiting friends\n";
              inFriends = false;

              //clear friends ttf text
              SDL_DestroyTexture(textureFriends);

              //clear friends background
              SDL_DestroyTexture(friendScreen);
              SDL_RenderClear(renderer);
            }
          }
        } //end if wifi is clicked
      } //end if click


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
          //inputText=" ";
          renderText = true;
          send_text = true;
          text = inputText;
          inputText = "";
          //std::cout << "Input was 'sent': " << text << "\n";
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

    temp = master;
    select(sockfd+1, &temp, nullptr, nullptr, &tv);


    //if the socket has information to receive
    if (FD_ISSET(sockfd, &temp)){ //if(text.size() > 0){
      memset(&buf, 0, sizeof buf);
      int bytesReceived = recv(sockfd, buf, 4096, 0);//this guy blocks until it receives from the server
      if(bytesReceived > 0){
        text = string(buf, 0, bytesReceived);
        renderText = true;
      }
      else{
        break;
      }
    }
    //if the user has pressed enter
    else if (send_text){
      if(text.size() > 0){
        text = this_user.get_username() + ": " + text;

        int sendResult = send(sockfd, text.c_str(), text.size() + 1, 0);
        send_text = false;

        if(sendResult <= 0){
          break;
        }
      }
    }

    //Rerender text if needed
    if(renderText) {

      //Text is not empty
      if(inputText != "") {
        //Render new text. Create surface first and then texture
        surfaceInputText = TTF_RenderText_Blended_Wrapped(fontStyle, inputText.c_str(), textWhite, 1215);
        textureInputText = SDL_CreateTextureFromSurface(renderer, surfaceInputText);
      }
      //Text is empty
      else {
        surfaceInputText = TTF_RenderText_Blended_Wrapped(fontStyle, " ", textWhite, 1215);
        textureInputText = SDL_CreateTextureFromSurface(renderer, surfaceInputText);
      }
      SDL_QueryTexture(textureInputText, NULL, NULL, &textW, &textH);
      inputRect.w = textW;
      inputRect.h = textH;
      blankRect.h = textH;
      if(textW>blankRect.w)
        blankRect.w = textW; // controls the width of the rect

      //update messages from server
      update_messages(renderer, text);
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

    // Close Chat room
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
  surfaceInputText = TTF_RenderText_Blended_Wrapped(fontStyle, inputText.c_str(), textWhite, 1215);
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

void update_messages(SDL_Renderer* renderer, string textFromServer){

  //take past messages from server and make them a surface --> texture

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
