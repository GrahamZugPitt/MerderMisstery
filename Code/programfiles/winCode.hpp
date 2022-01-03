#ifndef __WINCODE__
#define __WINCODE__

#include "common.hpp"

void runWinScreen(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer);
void runLoseScreen(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer);

#endif
