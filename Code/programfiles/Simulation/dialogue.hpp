#ifndef __DIALOGUE__H
#define __DIALOGUE__H

#include "simulation.hpp"
#include "common.hpp"
#include "../NPC_Gen/npc.hpp"
#include "Simulation/dialogue.hpp"
#include "winCode.hpp"
#include "main_helper.hpp"
#include <cstring>

int dialogue(NPClite* town, SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, NPC *discussWith);
bool goodMurder(NPClite *town);

#endif
