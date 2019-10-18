/*
* For testing ( all debugging enabled):
*	compile with:  make npcd
* 	execute with ./npcd
* 
*	Notes:
*	The log file gets overwritten with every new execution
*/

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>

#include "bmp_edit.hpp"
#include "npc.hpp"



//Constructor
NPC::NPC(std::string name, std::string spriteFileName, unsigned int dLust, unsigned int dLoyal, unsigned int dWrath) :mood{0}, murderiness{0},
													name{name}, spriteFileName{spriteFileName} {
	

	spriteRow = 0;
	prevTime = 0;
	goingRight = true;
	// Seed RNG
	srand(time(NULL));
	// set relationships to random value
	for (int i = 0; i<NUM_NPCS; i++){
		relationships[i] = rand() % 201 - 100;
	}
	// set personality to random values
	for (auto trait: traits){
		personality.insert({trait, rand() % 201 - 100});
	}
	
	std::string oname = spriteFileName.substr(0,spriteFileName.length()-4) + "m.bmp";
	
	BMPMod img(spriteFileName, oname);
	if (personality[traits[0]] >= 25){
		std::cout << "lust colors swapped" << std::endl;
		img.swapColor((unsigned int)0xFF00FF00, (unsigned int)0xFFE1B09F);
	} else {
		img.swapColor((unsigned int)0xFF00FF00, dLust);
	}
	if (personality[traits[1]] >= 25){
		std::cout << "disloyalty colors swapped" << std::endl;
		img.swapColor((unsigned int) 0xFF0000FF, (unsigned int)0xFF000000);
	} else {
		img.swapColor((unsigned int)0xFF0000FF, dLoyal);
	}
	if (personality[traits[3]] >= 25){
		std::cout << "wrath colors swapped" << std::endl;
		img.swapColor((unsigned int) 0xFFFF0000, (unsigned int)0xFFEE1111);
	} else {
		img.swapColor((unsigned int) 0xFFFF0000, dWrath);
	} if (personality[traits[2]] >= 25){
		std::cout << "greed sprite used" << std::endl;
		spriteRow += 1; //use second or fourth row
	} if (personality[traits[4]] >= 25){
		std::cout << "Dishonesty sprite used" << std::endl;
		spriteRow += 2;
	}	
	spriteFileName = oname;
	#ifdef LOGGING
	std::string str = "--Constructor Called--\n";
	str += toString();
	log(lfName, str);
	#endif
	#ifdef VERBOSE
	std::cout << str << std::endl;
	#endif
	printf("exiting\n");
}
// Initializes the sprite size and position on the map	
int NPC::initSprite(SDL_Renderer *renderer, int sizeX, int sizeY, int posX, int posY){
	crop.x = 7; //TODO: fix this offset in the sprite sheet
	crop.y = spriteRow * sizeY;
	crop.w = sizeX;
	crop.h = sizeY;
	position.x = posX;
	position.y = posY;
	position.w = sizeX;
	position.h = sizeY;
	std::string oname = spriteFileName.substr(0,spriteFileName.length()-4) + "m.bmp";
    SDL_Surface *surface = IMG_Load(oname.c_str());
    
    //Check if we can pull PlayerSpriteSheet.png
    if (surface == NULL)
        printf("Error loading Player Sprite Sheet!");
    else
    {
        //Create texture from player sprite surface we just created
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        
        //Check if texture was able to be made
        if (texture == NULL){
			printf("Error creating texture from player sprite sheet surface!");
        	return 0;
        }
    }
    
    //Free memory of player sprite surface
    SDL_FreeSurface(surface);
    return 1;
}
// A method to print NPC info to the console
void NPC::print(){
	std::cout << toString() << std::endl; 
}

// Create a string representation on the NPC
std::string NPC::toString(){
	//  This works but may be inefficent
	//  If a bottle neck is noticed we could redo this with sprintf equivalent
	std::string str = "Name: " + name;
	str += "\nSprite file: " + spriteFileName;
	str += "\nPersonality:\n";
	for( auto trait: traits){
		str += "  " + trait + ": " + std::to_string(personality[trait]) + "\n";
	}
	str += "\nEmotional State: ";
	str += "\n  Mood: " + std::to_string(mood);
	str += "\n  Murderiness: " + std::to_string(murderiness);
	str += "\nRelationships:";
	int i = 0;
	for (i = 0; i < NUM_NPCS; i++){
		str += "\n  " + names[i] + ": " + std::to_string(relationships[i]);
	}
	str += "\n";
	return str;
}

void NPC::renderToScreen(SDL_Renderer *renderer, float timechange){
	prevTime += timechange;
	if(prevTime >= 0.5f){
		prevTime = 0.0;
		// use one of first two rows
		// goes left, center, right, repeat
		if( spriteRow < 2){
		crop.x += NPC_WIDTH;
			if (crop.x > 2 * NPC_WIDTH + 7){
				crop.x = 7;
			}
		}
		// use one of last two rows
		// goes left, center, right, center, repeat 
		else {
			if(goingRight){
				crop.x += NPC_WIDTH;
				if(crop.x > 2*NPC_WIDTH + 7){
					crop.x -= NPC_WIDTH;
					goingRight = false;
				}
			} else {
				crop.x -= NPC_WIDTH;
				if (crop.x < 7){
					crop.x = 7;
					goingRight = true;
				}
			}
		}
	}
	if(renderer != NULL && texture != NULL){
		SDL_RenderCopy(renderer, texture, &crop, &position);
	}
}

// prints the string str to the file named by filename
int NPC::log(std::string filename, std::string str){
	int result = 0;
	std::ofstream logfile;
	logfile.open(filename, std::ios::app);
	if (logfile.is_open()){
		logfile << str;
		logfile.close();
		result = 1;
	}
	return result;
}

// Destructor
NPC::~NPC(){
	SDL_DestroyTexture(texture);
	texture = NULL;
	#ifdef LOGGING
	std::string str = "--Destructor Called--\n";
	str += "Name: " + name + "\n";
	log(lfName, str);
	#endif
	#ifdef VERBOSE
	std::cout << str << std::endl;
	#endif
}

// Accessors
std::string NPC::getName(){
	return name;
}

std::string NPC::getSpriteName(){
	return spriteFileName;
}
/*
int main(){
	unsigned int wrath, lust, loyalty;
	wrath = 0xFF6d4433;  	// Apron
	lust = 0xFF11EE22;		// shirt
	loyalty = 0xFF888899;	// pants
	NPC a("HI", "Art/NPCs/Blacksmith.bmp", lust, loyalty, wrath);
	std::cout << a.toString() << std::endl;
}
*/