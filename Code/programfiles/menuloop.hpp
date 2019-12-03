#ifndef GLOBALVAR_COMMON
#define GLOBALVAR_COMMON
#include "common.hpp"
#endif

#include "gameloop.hpp"
#include "main_helper.hpp"
#include "chat.hpp"
#include "chatLogin.hpp"
#include "seed.hpp"
#include "credits.hpp"
#include "info.hpp"
#include  <fstream>

std::string menuloop(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer);
void render_menu(SDL_Renderer* renderer);
