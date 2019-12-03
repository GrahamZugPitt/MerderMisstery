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
/* Creates an Instance of an NPC
	Args:
		name: the NPCs name
		spriteFileName: the name of the bitmap file to use for the npc
		dLust: the color to usefor the lust section if the npc is not lustful
		dLoyal: the color to usefor the disloyalty section if the npc is not disloyal
		dWrath: the color to usefor the wrath section if the npc is not wrathful
		NOTE: For Linux these values should be 0xAARRGGBB where AA is alpha RR is red BB is blue and GG is green
		This may not work properly on mac since mac forces bitmaps into its own masks for the color channels
*/
NPC::NPC() :mood{0}, murderiness{0} {

	spriteRow = 0;
	prevTime = 0;
	goingRight = true;
	// Seed RNG
	// set relationships to random value
	for (int i = 0; i<NUM_NPCS; i++){
		relationships[i] = rand() % 201 - 100;
	}
	// set personality to random values
	for (auto trait: traits){
		personality.insert({trait, rand() % 201 - 100});
	}

	#ifdef LOGGING
	std::string str = "--Constructor Called--\n";
	str += toString();
	log(lfName, str);
	#endif
	#ifdef VERBOSE
	std::cout << str << std::endl;
	#endif
}

/* Initializes the sprite size and position on the map
	Args:
		renderer: a pointer to the renderer
		sizeX: the width of the sprite (60)
		sizeY: the height of the sprite (88)
		posX: the initial display x postion
		posY: the initial display y position
	Returns:
		1 on successful creation, 0 if the texture was unable to be made
*/
int NPC::initSprite(std::string charName, std::string spriteFileName, unsigned int dLust, unsigned int dLoyal, unsigned int dWrath,
					SDL_Renderer *renderer, int sizeX, int sizeY, int posX, int posY){
	name = charName;
	crop.x = 7 + rand() % 3; //TODO: fix this offset in the sprite sheet
	crop.y = spriteRow * sizeY;
	crop.w = sizeX;
	crop.h = sizeY;
	mapPos.x = posX;
	mapPos.y = posY;
	mapPos.w = sizeX;
	mapPos.h = sizeY;
	screenPos.x = -100;
	screenPos.y = -100;
	screenPos.w = sizeX;
	screenPos.h = sizeY;

	// Why on earth does 'new' return a pointer, what the f*ck
	NPCCollider = *(new Collidable(posX + NPCColliderXOffset, posY + NPCColliderYOffset, NPCColliderWidth, NPCColliderHeight));
	NPCConversationCollider = *(new Collidable(posX + ConvoColliderXOffset, posY + ConvoColliderYOffset, ConvoColliderWidth, ConvoColliderHeight));

	std::size_t found = spriteFileName.find_last_of("/");
	std::cout << spriteFileName.substr(found, spriteFileName.length()-4) << std::endl;
	std::string oname = spriteFileName.substr(0,spriteFileName.length()-4) + "m.bmp";

	BMPMod img(spriteFileName, oname);

	unsigned int fleshtone = fleshtones[rand() % 5];
	img.swapColor((unsigned int)0x80FFFFFF, fleshtone);

	if (personality[traits[0]] >= 25){
		std::cout << "lust colors swapped" << std::endl;
		img.swapColor((unsigned int)0xFF00FF00, fleshtone);
	} else {
		img.swapColor((unsigned int)0xFF00FF00, dLust);
	}
	if (personality[traits[1]] >= 25){
		std::cout << "disloyalty colors swapped" << std::endl;
		img.swapColor((unsigned int) 0xFF0000FF, DISLOYAL_GRAFX);
	} else {
		img.swapColor((unsigned int)0xFF0000FF, dLoyal);
	}
	if (personality[traits[3]] >= 25){
		std::cout << "wrath colors swapped" << std::endl;
		img.swapColor((unsigned int) 0xFFFF0000, WRATH_GRAFX);
	} else {
		img.swapColor((unsigned int) 0xFFFF0000, dWrath);
	} if (personality[traits[2]] >= 25){
		std::cout << "greed sprite used" << std::endl;
		spriteRow += 1; //use second or fourth row
	} if (personality[traits[4]] >= 25){
		std::cout << "Dishonesty sprite used" << std::endl;
		spriteRow += 2;
	}
	crop.y = spriteRow * sizeY;
	spriteFileName = oname;
	SDL_Surface *surface = IMG_Load(oname.c_str());

    //Check if the sprite sheet is loaded
    if (surface == NULL)
        printf("Error loading NPC Sprite Sheet!");
    else
    {
        //Create texture from player sprite surface we just created
				// And set The Render Scale Quality so we don't get fuzziness on zoom
			  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
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

/*Renders the npc sprite to the window
	Args:
		renderer: the renderer
		timechange: the time since the last frame was rendered
		posX: the absolute x position relative to the window to display the sprite
		posY: the absolute y position relative to the window to display the sprite
*/
void NPC::renderToScreen(SDL_Renderer *renderer, float timechange, SDL_Rect camPos){
	prevTime += timechange;
	if(prevTime >= 0.5f){
		prevTime = 0.0;
		// use one of first two rows
		// goes left, center, right, repeat
		if( spriteRow < 2){
		crop.x += NPC_SPRITE_WIDTH;
			if (crop.x > 2 * NPC_SPRITE_WIDTH + 7){
				crop.x = 7;
			}
		}
		// use one of last two rows
		// goes left, center, right, center, repeat
		else {
			if(goingRight){
				crop.x += NPC_SPRITE_WIDTH;
				if(crop.x > 2*NPC_SPRITE_WIDTH + 7){
					crop.x -= NPC_SPRITE_WIDTH;
					goingRight = false;
				}
			} else {
				crop.x -= NPC_SPRITE_WIDTH;
				if (crop.x < 7){
					crop.x = 7;
					goingRight = true;
				}
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
			}
		}
	}
	if(renderer != NULL && texture != NULL){
		screenPos.x = mapPos.x - camPos.x;
		screenPos.y = mapPos.y - camPos.y;
		if (screenPos.x >= -NPC_SPRITE_WIDTH && screenPos.x <= 1280 + NPC_SPRITE_WIDTH &&
			screenPos.y >= - NPC_SPRITE_HEIGHT && screenPos.y <= 720 + NPC_SPRITE_HEIGHT)
			SDL_RenderCopy(renderer, texture, &crop, &screenPos);
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

void NPC::ghostThisNPC(){
	isGhost = true;
	mapPos.x = 2064;
	mapPos.y = 80;
	SDL_SetTextureAlphaMod(texture, 100);
	NPCCollider = *(new Collidable(mapPos.x + NPCColliderXOffset, mapPos.y + NPCColliderYOffset, NPCColliderWidth, NPCColliderHeight));
	NPCConversationCollider = *(new Collidable(mapPos.x + ConvoColliderXOffset, mapPos.y + ConvoColliderYOffset, ConvoColliderWidth, ConvoColliderHeight));
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
