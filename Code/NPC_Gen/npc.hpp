#ifndef NPC_DEF_H_
#define NPC_DEF_H_


#include <SDL2/SDL_image.h>
#include <string>
#include <map>
// This is just a dummy event until we can include an actual event
class Event{
	public:
		std::string event;
		Event(std::string str) :event{str}{};
		Event(){
			event = "NONE";
		}
};
const int NPC_WIDTH = 60;
const int NPC_HEIGHT = 88;

const std::string lfName = "NPC.log";
const int NUM_NPCS = 10;
//
const std::string traits[5] = {"Lust", "Disloyalty", "Greed", "Wrath", "Dishonesty"}; 

const std::string names[NUM_NPCS] = {"alpha", "bravo", "charlie", "delta", "echo", "foxtrot",
									"golf", "hotel", "india", "juliett"};

constexpr unsigned int fleshtones[5] = {0xffffdbac, 0xfff1c27d, 0xffe0ac69, 0xffc68642, 0xff8d5524};


const unsigned int BLACK = 0xFF000000;
const unsigned int GRAY = 0xFF888888;
const unsigned int WHITE = 0xFFFFFFFF;
const unsigned int BROWN = 0xFF985F0E;
const unsigned int DARK_GREEN = 0xFF009F42;
const unsigned int GREEN = 0xFF31E22a;
const unsigned int LIGHT_GREEN = 0xFF8CF187;
const unsigned int DARK_RED = 0xFF9A1414;
const unsigned int WRATH = 0xFFE12323;
const unsigned int LIGHT_RED = 0xFFED6363;
const unsigned int DARK_BLUE = 0xFF090968;
const unsigned int BLUE = 0xFF2A25D5;
const unsigned int LIGHT_BLUE = 0xFF1AF3EB;
const unsigned int ORANGE = 0xFFF69D1D;
const unsigned int PURPLE = 0xFFA606C8;
const unsigned int DISLOYAL = 0xFFDCF31A;

class NPC {
	public:
		// public fields
		// these are public to allow easy mutation by isd team
		// This may be bad practice but avoids the overhead of method calls.
		int mood;
		int murderiness;
		int relationships[NUM_NPCS];
		Event goal;
		SDL_Rect mapPos;
		SDL_Rect screenPos;
		SDL_Texture *texture;
		//Constructor
		//NPC(std::string name, std::string spriteFileName, unsigned int dLust, unsigned int dLoyal, unsigned int dWrath);
		NPC();
		int initSprite(std::string name, std::string spriteFileName, unsigned int dLust, unsigned int dLoyal, unsigned int dWrath,
						SDL_Renderer *renderer, int sizeX, int sizeY, int posX, int posY);
		void print();
		std::string toString();
		int log(std::string filename, std::string str);
		~NPC();
		std::string getName();
		std::string getSpriteName();
		void setName(std::string name);
		void setSpriteName(std::string filename);
		void renderToScreen(SDL_Renderer *renderer, float timechange, SDL_Rect camPos);
	private:
		int spriteRow;
		std::string name;
		std::string spriteFileName;
		std::map <std::string, int> personality;
		//SDL_Renderer *renderer;
		SDL_Rect crop;
		float prevTime;
		bool goingRight;

};
#endif