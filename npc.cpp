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


const std::string lfName = "NPC.log";
const int NUM_NPCS = 10;

const std::string names[NUM_NPCS] = {"alpha", "bravo", "charlie", "delta", "echo", "foxtrot",
									"golf", "hotel", "india", "juliett"};
class NPC {
	public:
		// public fields
		// these are public to allow easy mutation by isd team
		// This may be bad practice but avoids the overhead of method calls.
		float mood;
		float murderiness;
		float relationships[NUM_NPCS];

		//Constructor
		NPC(std::string name, std::string spriteFileName) :mood{0.0f}, murderiness{0.0f},
															name{name}, spriteFileName{spriteFileName} {
			for (int i = 0; i<NUM_NPCS; i++){
				relationships[i] = 0.0f;
			}
			personality.insert({"Openness", 1.00});
			personality.insert({"Conscientiousness", 0.99});
			personality.insert({"Extroversion", 0.98});
			personality.insert({"Agreeableness", 0.97});
			personality.insert({"Neuroticism", 0.96});
			
			#ifdef LOGGING
			std::string str = "--Constructor Called--\n";
			str += toString();
			log(lfName, str);
			#endif
			#ifdef VERBOSE
			std::cout << str << std::endl;
			#endif
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
			str += "\nPersonality:";
			str += "\n  Openness: " + std::to_string(personality["Openness"]);
			str += "\n  Conscientiousness: " + std::to_string(personality["Conscientiousness"]);
			str += "\n  Extroversion: " + std::to_string(personality["Extroversion"]);
			str += "\n  Agreeableness: " + std::to_string(personality["Agreeableness"]);
			str += "\n  Neuroticism: " + std::to_string(personality["Neuroticism"]);
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
		std::map <std::string, float> personality;
};
