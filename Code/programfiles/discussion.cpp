void enter_discussion(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, NPC *theNPC){


  // Load the phrases
  if(!(theNPC->isGhost)){
    TLsurfaceMessage = TTF_RenderText_Solid(TNR, TLString.c_str(), White);
    TRsurfaceMessage = TTF_RenderText_Solid(TNR, TRString.c_str(), White);
    BLsurfaceMessage = TTF_RenderText_Solid(TNR, BLString.c_str(), White);
    BRsurfaceMessage = TTF_RenderText_Solid(TNR, BRString.c_str(), White);
  }
  else{
    TLsurfaceMessage = TTF_RenderText_Solid(TNR, TLStringGhost.c_str(), White);
    TRsurfaceMessage = TTF_RenderText_Solid(TNR, TRStringGhost.c_str(), White);
    BLsurfaceMessage = TTF_RenderText_Solid(TNR, BLStringGhost.c_str(), White);
    BRsurfaceMessage = TTF_RenderText_Solid(TNR, BRStringGhost.c_str(), White);
  }
  // Also load the name
  CharNameSurface = TTF_RenderText_Solid(TNR, theNPC->getName().c_str(), Yellow);

  // Turn them into textures
  SDL_Texture *TLBox = SDL_CreateTextureFromSurface(renderer, TLsurfaceMessage);
  SDL_Texture *TRBox = SDL_CreateTextureFromSurface(renderer, TRsurfaceMessage);
  SDL_Texture *BLBox = SDL_CreateTextureFromSurface(renderer, BLsurfaceMessage);
  SDL_Texture *BRBox = SDL_CreateTextureFromSurface(renderer, BRsurfaceMessage);
  SDL_Texture *CharTex = SDL_CreateTextureFromSurface(renderer, CharNameSurface);
  // Create the position rectangles
  SDL_Rect TLBoxRect = {tl_x + 10, tl_y, boxw - 20, boxh};
  SDL_Rect TRBoxRect = {tr_x + 10, tr_y, boxw - 20, boxh};
  SDL_Rect BLBoxRect = {bl_x + 10, bl_y, boxw - 20, boxh};
  SDL_Rect BRBoxRect = {br_x + 10, br_y, boxw - 20, boxh};
  SDL_Rect NameBoxRect = {880 - (nameboxw / 2), 300, nameboxw, nameboxh};

  int selected = 1;

  //Clear screen
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

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

    if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]){
      if(selected == 3) selected = 1;
      if(selected == 4) selected = 2;
    }

    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]){
      if(selected == 2) selected = 1;
      if(selected == 4) selected = 3;
    }

    if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN]){
      if(selected == 1) selected = 3;
      if(selected == 2) selected = 4;
    }

    if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]){
      if(selected == 1) selected = 2;
      if(selected == 3) selected = 4;
    }

    if ((keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_RETURN]) && theNPC->isGhost){
      runWinScreen(e, quit, keyState, renderer);
    }

    if (keyState[SDL_SCANCODE_Q] || *quit){
      SDL_DestroyTexture(discussionBoxTex);
      SDL_DestroyTexture(selectedBoxTex);
      SDL_DestroyTexture(deselectedBoxTex);
      SDL_DestroyTexture(playerTex);
      SDL_DestroyTexture(TLBox);
      SDL_DestroyTexture(TRBox);
      SDL_DestroyTexture(BLBox);
      SDL_DestroyTexture(BRBox);
      SDL_DestroyTexture(CharTex);
      return;
    }

    SDL_RenderClear(renderer);

    renderTexture(renderer, discussionBoxTex, useless, 0, SCREEN_HEIGHT - h, w, h, false);
    draw_peoples(renderer, playerTex, npcTex);
    draw_boxes(selected, renderer, selectedBoxTex, deselectedBoxTex, w, h);
    draw_text(renderer, TLBox, TLBoxRect, TRBox, TRBoxRect, BLBox, BLBoxRect, BRBox, BRBoxRect);
    SDL_RenderCopy(renderer, CharTex, NULL, &NameBoxRect);

    //Update Screen
    SDL_RenderPresent(renderer);
  }
}
