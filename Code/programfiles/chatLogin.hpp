#include "common.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

void enter_login(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer);
void initialize_input_login(SDL_Renderer* renderer);
void update_render_login(SDL_Renderer* renderer);
