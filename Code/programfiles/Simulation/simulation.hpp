#ifndef __SIMHPP__
#define __SIMHPP__

#include "event.cpp"
#include "EventArray.cpp"
#include <cstdlib>
#include <ctime>

extern int rand_seed;
extern int CLOCK;

enum relationshipDelta{
	HATE = -75,
	SDISLIKE = -60,
	DESPISE = -30,
	DISLIKE = -25,
	DISCOMFORT = -10,
	NEUTRAL = 0,
	HAPPY = 5,
	LIKE = 10,
	LOVE = 25,
};

enum observer{
	SUBJECT,
	FIRSTHAND,
	SECONDHAND,
};

enum location{
	TOWN_HALL,
	FACTORY,
	MARKET,
	INN,
	CHURCH,
};
enum role{
	MAYOR,
	PRIEST,
	WORKER,
	MERCHANT,
	POLICE,
	INNKEEPER
};
enum trait{
	LUST,
	DISLOYALTY,
	GREED,
	WRATH,
	DISHONESTY,
	DEFAULT,
	HL,
};
enum event{
	CDATE, //0
	INTRODUCE, //1
	SOCIALIZE, //2
	ENVY, //3
	LIE, //4
	ASKFORDISTANCE, //5
	FLIRT, //6
	DATE, //7
	VERBALFIGHT, //8
	ROB, //9
	PHYSICALFIGHT, //10
	SEX, //11
	BREAKUP, //12
};

// A few constants used in the sim and dialogue classes
const int SEED = 0;
const int EVENT_MODE = 0;
const int DEBUG_MODE = 0;
const int OBSERVATION_MODE = 0;
const int HEARSAY_MODE = 0;
const int TOWN_SIZE = 12;
const int LOCATION = 5;
const int SELFCONTROL = 40;
const int MURDER_MODE = 0;
const int M = 982451653;
const int MULTIPLIER = 373587883;

class NPClite{
public:
	std::string name;
	int mood;
	EventArray memories;
	EventArray observations;
	EventArray hearSay;
	int relationships[12];
	int personality[5];
	int schedule[12];
	int murderiness;
	int role;
	bool didMurder;
	bool isDead;
	bool lockDown;

	NPClite();

	NPClite(std::string name, int role);
};

void simulation(NPClite* town, int givenSeed);
std::string locationConverter(int location);
bool hasDoneEventMutual(NPClite* town, int event, int firstNPC, int secondNPC);
bool wasInvolvedInEvent(NPClite* town, Event* e, int firstNPC);


#endif
