#include "common.hpp"

void enter_chat(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer);
void initialize_input(SDL_Renderer* renderer);
void update_messages(SDL_Renderer* renderer);
void update_render(SDL_Renderer* renderer);
