#ifndef __DISCUSSIONHPP__
#define __DISCUSSIONHPP__

#include "common.hpp"
#include "../NPC_Gen/npc.hpp"
#include "Simulation/dialogue.hpp"
#include "winCode.hpp"
#include "main_helper.hpp"
#include <cstring>

void enter_discussion(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, NPC *discussWith);

#endif
