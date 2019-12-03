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


    if ((keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_RETURN]) && theNPC->isGhost){
      runWinScreen(e, quit, keyState, renderer);
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
