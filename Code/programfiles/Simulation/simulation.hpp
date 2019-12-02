#ifndef __SIMHPP__
#define __SIMHPP__

#include "event.cpp"
#include "EventArray.cpp"
#include <cstdlib>
#include <ctime>

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

#endif
