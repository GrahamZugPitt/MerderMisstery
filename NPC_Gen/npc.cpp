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
#include <SDL2/SDL_image.h>
#include <time.h>
#include <stdlib.h>

#include "bmp_edit.hpp"
//#include "bmp_edit.cpp"


const std::string lfName = "NPC.log";
const int NUM_NPCS = 10;
//
const std::string traits[5] = {"Lust", "Disloyalty", "Greed", "Wrath", "Dishonesty"}; 

const std::string names[NUM_NPCS] = {"alpha", "bravo", "charlie", "delta", "echo", "foxtrot",
									"golf", "hotel", "india", "juliett"};

// This is just a dummy event until we can include an actual event
class Event{
	public:
		std::string event;
		Event(std::string str) :event{str}{};
		Event(){
			event = "NONE";
		}
};

class NPC {
	public:
		// public fields
		// these are public to allow easy mutation by isd team
		// This may be bad practice but avoids the overhead of method calls.
		int mood;
		int murderiness;
		int relationships[NUM_NPCS];
		Event goal;
		SDL_Surface *surface = nullptr;
		//Constructor
		NPC(std::string name, std::string spriteFileName, unsigned int dLust, unsigned int dLoyal, unsigned int dWrath) :mood{0}, murderiness{0},
															name{name}, spriteFileName{spriteFileName} {
			
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
			}
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
		
		// A method to print NPC info to the console
		void print(){
			std::cout << toString() << std::endl; 
		}

		// Create a string representation on the NPC
		std::string toString(){
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

		// prints the string str to the file named by filename
		int log(std::string filename, std::string str){
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
		~NPC(){
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
		std::string getName(){
			return name;
		}

		std::string getSpriteName(){
			return spriteFileName;
		}

	private:
		std::string name;
		std::string spriteFileName;
		std::map <std::string, int> personality;
};

int main(){
	unsigned int wrath, lust, loyalty;
	wrath = 0xFF6d4433;  	// Apron
	lust = 0xFF11EE22;		// shirt
	loyalty = 0xFF888899;	// pants
	NPC a("HI", "Art/NPCs/Blacksmith.bmp", lust, loyalty, wrath);
	std::cout << a.toString() << std::endl;
}