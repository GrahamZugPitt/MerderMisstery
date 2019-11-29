#ifndef __DISCUSSION__
#define __DISCUSSION__

#include "common.hpp"
#include "../NPC_Gen/npc.hpp"
#include "winCode.hpp"
#include <cstring>

void enter_discussion(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, NPC *discussWith);

#endif
