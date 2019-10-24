#ifndef GLOBALVAR_COMMON
#define GLOBALVAR_COMMON
#include "common.hpp"
#endif

void gameloop(SDL_Event e, bool quit, int curr_time, int last_time, float time_change, const Uint8 *keyPressed, SDL_Renderer* renderer);
