#include "common.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

using namespace std;

void enter_chat(SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer);
void initialize_input(SDL_Renderer* renderer);
void update_messages(SDL_Renderer* renderer, string textFromServer);
void update_render(SDL_Renderer* renderer);
