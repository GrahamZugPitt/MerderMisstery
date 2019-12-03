#ifndef __GAMELOOPHPP__
#define __GAMELOOPHPP__

#include "common.hpp"

#include "main_helper.hpp"
#include "Player.hpp"
#include "chat.hpp"
#include "inventory.hpp"
#include "discussion.hpp"
#include "worldObjects.hpp"
#include "Building.hpp"
#include "../NPC_Gen/npc.hpp"
#include "Simulation/simulation.hpp"

#include "CyanBuilding.hpp"

#include "BlueBuilding.hpp"
#include "GreenBuilding.hpp"
#include "YellowBuilding.hpp"
#include "RedBuilding.hpp"
#include <time.h>
#include <fstream>

void gameloop(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, bool farnan, std::string seed, int player_x, int player_y, int itemList[4]);

#endif
