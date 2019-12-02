#ifndef GLOBALVAR_COMMON
#define GLOBALVAR_COMMON
#include "common.hpp"
#endif

void gameloop(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, bool farnan, int player_x, int player_y, int itemList[4]);
