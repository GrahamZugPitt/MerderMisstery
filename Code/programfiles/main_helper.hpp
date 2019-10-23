//
// Setup to define everything for our main file
// Game Design
//
// Team Alpha Game Collective
//

//Include Statements
#ifndef GLOBALVAR_COMMON
#define GLOBALVAR_COMMON
#include "common.hpp"
#endif

//Function Declarations
bool initialize();
SDL_Texture* loadFiles(std::string name, SDL_Renderer *renderer);
void setCamera();
void clean();

