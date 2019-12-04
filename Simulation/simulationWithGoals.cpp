#include "event.cpp"
#include "EventArray.cpp"
#include <cstdlib>
#include <ctime>


const int SEED = 0;
const int EVENT_MODE = 0;
const int DEBUG_MODE = 1;
const int OBSERVATION_MODE = 0;
const int HEARSAY_MODE = 0;
const int TOWN_SIZE = 12;
const int LOCATION = 5;
const int SELFCONTROL = 40;
const int MURDER_MODE = 1;
const int M = 982451653;
const int MULTIPLIER = 373587883;
unsigned int seed = 1; 
int CLOCK = 0;

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


// Code for the custom random number generator that's platfor independent

int z_rand(){
	seed = (seed*MULTIPLIER + 1) % M;
	if(seed < 0)
		seed = seed*-1;		
	return seed;
}

class NPClite{

public:
	std::string name;
	int mood;
	EventArray memories;
	EventArray observations;
	EventArray hearSay;
	int relationships[TOWN_SIZE];
	int personality[5];
	int schedule[12];
	int goal[12];
	int murderiness;
	int role;
	bool didMurder;
	bool isDead;
	bool lockDown;

	NPClite(){
		name = "";
		role = -1;
		for(int i = 0; i < TOWN_SIZE; i++)
			relationships[i] = 0;
		for(int i = 0; i < 5; i++){
			personality[i] = (z_rand() % 101);
		}
		for(int i = 0; i < TOWN_SIZE; i++){
			goal[i] = -1;
		}
		murderiness = 0;
		mood = 0;
		didMurder = false;
		isDead = false;
		lockDown = false;
	}	

	NPClite(std::string name, int role){ //constructor
		this->name = name;
		this->role = role;
		for(int i = 0; i < TOWN_SIZE; i++)
			relationships[i] = 0;
		for(int i = 0; i < 5; i++){
			personality[i] = (z_rand() % 101);
		}
		memories.wipe();
		observations.wipe();
		hearSay.wipe();
		for(int i = 0; i < TOWN_SIZE; i++){
			goal[i] = -1;
		}		
		murderiness = 10;
		mood = 0;
		didMurder = false;
		isDead = false;
		lockDown = false;

		switch(role){ //assign schedules
			case MAYOR:
				schedule[0] = MARKET;
				schedule[1] = MARKET;
				schedule[2] = FACTORY;
				schedule[3] = INN;
				schedule[4] = TOWN_HALL;
				schedule[5] = MARKET;
				schedule[6] = CHURCH;
				schedule[7] = INN;
				schedule[8] = CHURCH;
				schedule[9] = MARKET;
				schedule[10] = TOWN_HALL;
				schedule[11] = MARKET;
				break;
			case PRIEST:
				schedule[0] = FACTORY;
				schedule[1] = MARKET;
				schedule[2] = FACTORY;
				schedule[3] = INN;
				schedule[4] = TOWN_HALL;
				schedule[5] = MARKET;
				schedule[6] = CHURCH;
				schedule[7] = INN;
				schedule[8] = CHURCH;
				schedule[9] = MARKET;
				schedule[10] = TOWN_HALL;
				schedule[11] = MARKET;
				break;
			case WORKER:
				schedule[0] = FACTORY;
				schedule[1] = MARKET;
				schedule[2] = FACTORY;
				schedule[3] = INN;
				schedule[4] = TOWN_HALL;
				schedule[5] = MARKET;
				schedule[6] = FACTORY;
				schedule[7] = INN;
				schedule[8] = CHURCH;
				schedule[9] = MARKET;
				schedule[10] = TOWN_HALL;
				schedule[11] = INN;
				break;
			case MERCHANT:
				schedule[0] = MARKET;
				schedule[1] = MARKET;
				schedule[2] = INN;
				schedule[3] = INN;
				schedule[4] = MARKET;
				schedule[5] = MARKET;
				schedule[6] = INN;
				schedule[7] = INN;
				schedule[8] = CHURCH;
				schedule[9] = MARKET;
				schedule[10] = TOWN_HALL;
				schedule[11] = MARKET;
				break;
			case POLICE:
				schedule[0] = FACTORY;
				schedule[1] = MARKET;
				schedule[2] = FACTORY;
				schedule[3] = INN;
				schedule[4] = TOWN_HALL;
				schedule[5] = MARKET;
				schedule[6] = FACTORY;
				schedule[7] = INN;
				schedule[8] = CHURCH;
				schedule[9] = MARKET;
				schedule[10] = TOWN_HALL;
				schedule[11] = INN;
				break;
			case INNKEEPER:
				schedule[0] = MARKET;
				schedule[1] = MARKET;
				schedule[2] = INN;
				schedule[3] = INN;
				schedule[4] = MARKET;
				schedule[5] = MARKET;
				schedule[6] = INN;
				schedule[7] = INN;
				schedule[8] = CHURCH;
				schedule[9] = MARKET;
				schedule[10] = TOWN_HALL;
				schedule[11] = INN;
				break;

		}

	}
};

//converts the integer value associated with an event to a string
std::string eventConverter(int event){
	switch(event){
		case CDATE:
			return "is in a happy relationship with";
		case FLIRT:
			return "flirted with";
		case INTRODUCE:
			return "met";
		case DATE:
			return "began dating";
		case BREAKUP:
			return "broke up with";
		case SEX:
			return "walked off to a private place holding hands with";
		case SOCIALIZE:
			return "gossiped with";
		case ENVY:
			return "glared enviously at";
		case ROB:
			return "robbed";
		case VERBALFIGHT:
			return "got in an aggressive argument with";
		case PHYSICALFIGHT:
			return "began throwing hands with";
		case LIE:
			return "lied to";
		case ASKFORDISTANCE:
			return "asked for distance from";
		}
	return "performed an unperformable action on";
}
//converts the integer value associated with a location to a string
std::string locationConverter(int location){
	switch(location){
		case TOWN_HALL:
			return "the town hall";
		case FACTORY:
			return "the factory";
		case MARKET:
			return "the market";
		case INN:
			return "the inn";
		case CHURCH:
			return "the church";
		}
	std::cout << location;
	return "a void in which two people should never meet.";
}
Event* getLastEvent(NPClite* town, int person){
	return town[person].memories.getMemory(town[person].memories.getSize() - 1);
}

void printEvent(Event* e){
	std::cout << e->npcName1 << " " << eventConverter(e->event) << " " << e->npcName2 << " at time " << e->time << " at " << locationConverter(e->location) << ". \n";
}
//checks if NPC one has done an event to NPC two (Sometimes, this gurantees that they have both done the event to each other).
bool hasDoneEvent(NPClite* town, int event, int firstNPC, int secondNPC){
	for(int i = 0; i < town[firstNPC].memories.getSize(); i++){
		if (((town[firstNPC].memories.getMemory(i)->npcName2.compare(town[secondNPC].name)) == 0) && town[firstNPC].memories.getMemory(i)->event == event)
			return true;
	}
	return false;
}

bool hasDoneEventMutual(NPClite* town, int event, int firstNPC, int secondNPC){
	return (hasDoneEvent(town,event,firstNPC,secondNPC) || hasDoneEvent(town,event,secondNPC,firstNPC));
}

bool isEventType(NPClite* town, Event* e, int eventType){
	if(eventType == e->event)
		return true;
	return false;
}

int findObserver(NPClite* town, int firstNPC, int secondNPC){
	int obs[TOWN_SIZE];
	int obsCount = 0;
	for(int i = 0; i < TOWN_SIZE; i++){
		obs[i] = -1;
	}
	for(int i = 0; i < TOWN_SIZE; i++){
		if(i != firstNPC && i != secondNPC && town[i].schedule[CLOCK%12] == town[firstNPC].schedule[CLOCK%12]){
		obs[i] = i;
		obsCount++;
		}
	}
	if(!obsCount)
		return - 1;
	int theObserver = z_rand() % obsCount;
	for(int i = 0; i < TOWN_SIZE; i++){
		if(i != firstNPC && i != secondNPC && town[i].schedule[CLOCK%12] == town[firstNPC].schedule[CLOCK%12]){
		if(!theObserver)
			return i;
		theObserver--;
		}
	}
return -1;
}

bool sameLocation(NPClite* town, int npc1, int npc2){
	return (town[npc1].schedule[CLOCK%12] == town[npc2].schedule[CLOCK%12]);
}

void changeRelationship(NPClite* town, int firstNPC, int secondNPC, int deltaRelationship){
	town[firstNPC].relationships[secondNPC] = town[firstNPC].relationships[secondNPC] + deltaRelationship;
	if(town[firstNPC].relationships[secondNPC] > 100)
		town[firstNPC].relationships[secondNPC] = 100;
	if(town[firstNPC].relationships[secondNPC] < -100)
		town[firstNPC].relationships[secondNPC] = -100;
}

void changeMood(NPClite* town, int person, int deltaMood){ //I am aware this function isn't used. We are planning on continuing work on the game, and I thought we may want to change how we approach the use of mood in the future.
	town[person].mood = town[person].mood + deltaMood;
}
//changes an observers memory. Must be run immediately after a memory was added to firstNPC
void observe(NPClite* town, int firstNPC, int observer){
	Event* e2 = new Event(getLastEvent(town, firstNPC), FIRSTHAND);
	town[observer].observations.add(e2);
}

//changes listeners memory.
void gossip(NPClite* town, Event* e, int listener){
	Event* e2 = new Event(e, CLOCK, SECONDHAND);
	town[listener].hearSay.add(e2);
}
//checks to see if someone observed an event first hand
bool didObserve(NPClite* town,  int observer, int firstNPC, int secondNPC){
	return (town[firstNPC].schedule[CLOCK%12] == town[observer].schedule[CLOCK%12] && firstNPC != observer && secondNPC != observer);
}
bool initiatedEvent(NPClite* town, Event* e, int firstNPC){
	return (!e->npcName1.compare(town[firstNPC].name));
}

bool wasInvolvedInEvent(NPClite* town, Event* e, int firstNPC){
	return (!e->npcName1.compare(town[firstNPC].name) || !e->npcName2.compare(town[firstNPC].name));
}
//adds memory two people experienced first hand
void addMemory(NPClite* town, int eventType, int firstNPC, int secondNPC){
	Event* e = new Event(town[firstNPC].name, town[secondNPC].name, eventType, CLOCK, town[firstNPC].schedule[CLOCK%12], SUBJECT);

	town[firstNPC].memories.add(e);
	town[secondNPC].memories.add(e);
}
//the introduce event
void introduce(NPClite* town, int firstNPC, int secondNPC){
	addMemory(town,INTRODUCE,firstNPC,secondNPC);
	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
	int observer = findObserver(town,firstNPC,secondNPC);
	if(observer != -1)
		observe(town, firstNPC, observer);
}

//The flirting event
void flirt(NPClite* town, int firstNPC, int secondNPC){
	addMemory(town, FLIRT, firstNPC, secondNPC);
	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
	int observer = findObserver(town,firstNPC,secondNPC);
	changeRelationship(town,firstNPC,secondNPC,HAPPY);
	changeRelationship(town,secondNPC,firstNPC,HAPPY);
	if(observer != -1){
		observe(town, firstNPC, observer);
		if(hasDoneEvent(town,DATE,observer,firstNPC) || hasDoneEvent(town,DATE,firstNPC,observer)){
			changeRelationship(town,observer,firstNPC,DISCOMFORT);
			changeRelationship(town,observer,secondNPC,DISLIKE);
		}
		if(hasDoneEvent(town,DATE,observer,secondNPC) || hasDoneEvent(town,DATE,secondNPC,observer)){
			changeRelationship(town,observer,secondNPC,DISCOMFORT);
			changeRelationship(town,observer,firstNPC,DISLIKE);
		}
	}
}
//the dating event
void date(NPClite* town, int firstNPC, int secondNPC){
	int observer = findObserver(town,firstNPC,secondNPC);
	if(!hasDoneEvent(town,DATE,firstNPC,secondNPC) && !hasDoneEvent(town,DATE,firstNPC,secondNPC)){
		addMemory(town, DATE, firstNPC, secondNPC);
		changeRelationship(town,firstNPC,secondNPC,LOVE);
		changeRelationship(town,secondNPC,firstNPC,LOVE);
	}else {
		addMemory(town, CDATE, firstNPC, secondNPC);
		changeRelationship(town,firstNPC,secondNPC,LIKE);
		changeRelationship(town,secondNPC,firstNPC,LIKE);
	}
	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
	if(observer != -1){
		observe(town, firstNPC, observer);
		if(hasDoneEventMutual(town,DATE,observer,firstNPC)){
			changeRelationship(town,observer,firstNPC,DISLIKE);
			changeRelationship(town,observer,secondNPC,DISLIKE);
		}
		if(hasDoneEventMutual(town,DATE,observer,secondNPC)){
			changeRelationship(town,observer,secondNPC,DISLIKE);
			changeRelationship(town,observer,firstNPC,DISLIKE);
		}
	}
}
//Checks if an NPC is dating someone
bool isDating(NPClite* town, int firstNPC){
	bool dating = false;
	for(int i = 0; i < town[firstNPC].memories.getSize(); i++){
		if(isEventType(town, town[firstNPC].memories.getMemory(i),DATE))
			dating = true;
		if(isEventType(town, town[firstNPC].memories.getMemory(i),BREAKUP))
			dating = false;
	}
	return dating;
}
//the break up event
void breakUp(NPClite* town, int firstNPC, int secondNPC){
	addMemory(town, BREAKUP, firstNPC, secondNPC);
	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
	changeRelationship(town,firstNPC,secondNPC,SDISLIKE);
	changeRelationship(town,secondNPC,firstNPC,SDISLIKE);

	int observer = findObserver(town,firstNPC,secondNPC);
	if(observer != -1){
		observe(town, firstNPC, observer);
	}
}
//OwO what's this?
void sex(NPClite* town, int firstNPC, int secondNPC){
	addMemory(town, SEX, firstNPC, secondNPC);
	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
	changeRelationship(town,firstNPC,secondNPC,LIKE);
	changeRelationship(town,secondNPC,firstNPC,LIKE);
	int observer = findObserver(town,firstNPC,secondNPC);
	if(observer != -1){
		observe(town, firstNPC, observer);
		if(hasDoneEventMutual(town,DATE,firstNPC,observer)){
			changeRelationship(town,observer,firstNPC,HATE);
			changeRelationship(town,observer,secondNPC,HATE);
		}
		if(hasDoneEventMutual(town,DATE,secondNPC,observer)){
			changeRelationship(town,observer,secondNPC,HATE);
			changeRelationship(town,observer,firstNPC,HATE);
		}
	}
}
//The envy event
void envy(NPClite* town, int firstNPC, int secondNPC){
	addMemory(town,ENVY,firstNPC,secondNPC);
	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
	changeRelationship(town,firstNPC,secondNPC,DISCOMFORT);
	int observer = findObserver(town,firstNPC,secondNPC);
	if(observer != -1)
		observe(town, firstNPC, observer);
}

void rob(NPClite* town, int firstNPC, int secondNPC){
	addMemory(town,ROB,firstNPC,secondNPC);
	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
	changeRelationship(town,firstNPC,secondNPC,DISLIKE);
	if(hasDoneEvent(town,ASKFORDISTANCE,secondNPC,firstNPC)){
		changeRelationship(town,secondNPC,firstNPC,HATE);
	}
	int observer = findObserver(town,firstNPC,secondNPC);
	if(observer != -1){
		observe(town, firstNPC, observer);
		changeRelationship(town,observer,firstNPC,DISCOMFORT);
	}
}

void verbalFight(NPClite* town, int firstNPC, int secondNPC){
	addMemory(town,VERBALFIGHT,firstNPC,secondNPC);
	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
	changeRelationship(town,firstNPC,secondNPC,DISLIKE);
	changeRelationship(town,secondNPC,firstNPC,DISLIKE);
	int observer = findObserver(town,firstNPC,secondNPC);
	if(observer != -1){
		observe(town, firstNPC, observer);
	}
}

void physicalFight(NPClite* town, int firstNPC, int secondNPC){
	addMemory(town,PHYSICALFIGHT,firstNPC,secondNPC);
	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
	changeRelationship(town,firstNPC,secondNPC,DESPISE);
	changeRelationship(town,secondNPC,firstNPC,DESPISE);
	int observer = findObserver(town,firstNPC,secondNPC);
	if(observer != -1){
		changeRelationship(town,observer,firstNPC,DISCOMFORT);
		observe(town, firstNPC, observer);
	}
}

void gossipRelationshipChange(NPClite* town, Event* e, int firstNPC){
	int npc2 = -1;
	int npc3 = -1;
	for(int i = 0; i < TOWN_SIZE; i++){
		if(!e->npcName1.compare(town[i].name))
			npc2 = i;
	}
	for(int i = 0; i < TOWN_SIZE; i++){
		if(!e->npcName1.compare(town[i].name))
			npc3 = i;
	}
	if(hasDoneEventMutual(town,DATE,firstNPC,npc2) || hasDoneEventMutual(town,DATE,firstNPC,npc3)){
		if(e->event == SEX){
			changeRelationship(town,firstNPC,npc2,DESPISE);
			changeRelationship(town,firstNPC,npc3,DESPISE);
		}
		if(e->event == FLIRT || e->event == DATE){
			changeRelationship(town,firstNPC,npc2,DISLIKE);
			changeRelationship(town,firstNPC,npc3,DISLIKE);
		}
	}
}

int findRandomDisliked(NPClite* town, int murderer, int victim){
	int dislikeCount = 0;
	int relation[TOWN_SIZE];
	for(int i = 0; i < TOWN_SIZE; i++){
		relation[i] = -1;
	}
	for(int i = 0; i < TOWN_SIZE; i++){
		if(town[i].relationships[victim] < 0 && i != murderer){ //NOTE: The murderer has "momentary omniscience," here. They know if someone dislikes the victim even if they've never interacted with them. This allows the murderer to tell more convincing lies, and it isn't a very noticable break in the narrative, as other NPCs can say things about people they've observed and haven't met. Nonetheless, it is something worth mentioning moving forward. The omniscience could be canceled by adding a "&& hasDoneEventMutual(town,INTRODUCE,i,murderer)" restriction to the above if statement.
			dislikeCount++;
			relation[i] = i;
		}
	}
	if(dislikeCount){
		int person = (z_rand() % dislikeCount) + 1;
		for(int i = 0; i < TOWN_SIZE; i++){
			if(relation[i] != -1)
				person--;
			if(!person)
				return relation[i];
		}
	}
	int person = z_rand() % TOWN_SIZE; //murderer just started lying if no one but them dislikes the murdered
	while(person == victim || person == murderer)
		person = z_rand() % TOWN_SIZE;
	return person;
}

void makeUpCoverUp(NPClite* town, int murderer, int victim){
	int clockModifierMeta = -1;
	int numberOfLies = (town[murderer].personality[DISHONESTY] / 25) + 1;
		if(numberOfLies > 3)
			numberOfLies = 3;
	for(int i = 0; i < numberOfLies; i++){
		int npc1 = findRandomDisliked(town,murderer,victim);	
		int lieEvent = (z_rand() % 4) + 1;
		switch(lieEvent){
			case 1: lieEvent = VERBALFIGHT; break;
			case 2: lieEvent = PHYSICALFIGHT; break;
			case 3: lieEvent = ROB; break;
			case 4: lieEvent = ASKFORDISTANCE; break;
		}
		
		int clockModifier = z_rand() % ((CLOCK % 4) + 1);
		if(clockModifierMeta == -1)
			clockModifierMeta = clockModifier;
		if(clockModifierMeta == clockModifier)
			clockModifier++;	

		Event* lie = new Event(town[victim].name, town[npc1].name, lieEvent, CLOCK - clockModifier, town[npc1].schedule[(CLOCK - clockModifier)%12], FIRSTHAND);
	town[murderer].observations.add(lie);
	}
}

Event* makeUpLie(NPClite* town){

	int lieEvent = (z_rand() % 5) + 1;
	switch(lieEvent){
		case 1: lieEvent = FLIRT; break;
		case 2: lieEvent = VERBALFIGHT; break;
		case 3: lieEvent = PHYSICALFIGHT; break;
		case 4: lieEvent = ROB; break;
		case 5: lieEvent = SEX; break;
	}
		
	int npc1 = z_rand() % TOWN_SIZE;
	int npc2 = z_rand() % TOWN_SIZE;
	while(npc1 == npc2){
		npc2 = z_rand() % TOWN_SIZE;
	}
	Event* lie = new Event(town[npc1].name, town[npc2].name, lieEvent, CLOCK, town[npc1].schedule[CLOCK%12], SECONDHAND);
	return lie;
}

void lie(NPClite* town, int firstNPC, int secondNPC){
	addMemory(town,LIE,firstNPC,secondNPC);
	Event* e = makeUpLie(town);
	gossip(town,e,secondNPC);
	gossipRelationshipChange(town,e,secondNPC);
	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
}

void askForDistance(NPClite* town, int firstNPC, int secondNPC){
	addMemory(town,ASKFORDISTANCE,firstNPC,secondNPC);
	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
	changeRelationship(town,secondNPC,firstNPC,DESPISE);
	int observer = findObserver(town,firstNPC,secondNPC);
	if(observer != -1){
		observe(town, firstNPC, observer);
	}
}

Event* getGossip(NPClite* town, int firstNPC, int secondNPC){
	for(int i = BREAKUP; i > -1; i--){
		for(int j = town[firstNPC].hearSay.getSize(); j > -1; j--){
			if(town[firstNPC].hearSay.getMemory(j)->event == i && !initiatedEvent(town, town[firstNPC].hearSay.getMemory(j),secondNPC))
				return town[firstNPC].hearSay.getMemory(j);
		}
		for(int j = town[firstNPC].observations.getSize(); j > -1; j--){
			if(town[firstNPC].observations.getMemory(j)->event == i && !initiatedEvent(town, town[firstNPC].observations.getMemory(j),secondNPC))
				return town[firstNPC].observations.getMemory(j);
		}
		for(int j = town[firstNPC].memories.getSize(); j > -1; j--){
			if(town[firstNPC].memories.getMemory(j)->event == i && !initiatedEvent(town, town[firstNPC].memories.getMemory(j),secondNPC))
				return town[firstNPC].memories.getMemory(j);
		}
	}
	return NULL;
}

//the socialize event
void socialize(NPClite* town, int firstNPC, int secondNPC){
	addMemory(town,SOCIALIZE,firstNPC,secondNPC);
	gossip(town,getGossip(town,firstNPC,secondNPC),secondNPC);
	gossipRelationshipChange(town,getGossip(town,firstNPC,secondNPC),secondNPC);

	if(DEBUG_MODE)
		printEvent(getLastEvent(town, firstNPC));
	int observer = findObserver(town,firstNPC,secondNPC);
	if(observer != -1){
		observe(town, firstNPC, observer);
	}
}

void wakeUp(NPClite* town){
	if(DEBUG_MODE){
		std::cout << "DAY " << (CLOCK + 1)/4 << ":\n";
	}
	for(int i = 0; i < TOWN_SIZE; i++){
		town[i].murderiness = town[i].murderiness + town[i].mood;
		if(town[i].murderiness < -100)
			town[i].murderiness = -100;
		if(town[i].murderiness > 200)
			town[i].murderiness = 200;
		town[i].mood = (z_rand() % 3) - 1;
	}
}
//takes two NPCs as an input. Spits out the last event they had together.
Event* getLastInteraction(NPClite* town, int firstNPC, int secondNPC){
	for(int i = town[firstNPC].memories.getSize() - 1; i > -1; i--){
		if(town[firstNPC].memories.getMemory(i)->npcName1.compare(town[secondNPC].name) == 0 || town[firstNPC].memories.getMemory(i)->npcName2.compare(town[secondNPC].name) == 0){
			return town[firstNPC].memories.getMemory(i);
		}
	}
	return NULL;
}

int roll(NPClite* town, int person, int type){
	int chance;
	if(type == HL){
		chance = z_rand() % (SELFCONTROL + town[person].personality[DISHONESTY] + town[person].personality[LUST]);
		if(chance < town[person].personality[DISHONESTY])
			return DISHONESTY;
		else{
			chance = chance - town[person].personality[DISHONESTY];
			if(chance < town[person].personality[LUST])
				return LUST;
		}
		return DEFAULT;
	}

	if(type != DEFAULT){
		chance = z_rand () % (SELFCONTROL + town[person].personality[type]);
		if(chance < SELFCONTROL){
			return DEFAULT;
		} else return type;
	}
	int sumPersonality = 0;
	for(int i = LUST; i != DEFAULT; i++){
		sumPersonality = sumPersonality + town[person].personality[type];
	}
	sumPersonality = sumPersonality + 4*SELFCONTROL;
	chance = z_rand() % sumPersonality;
	for(int i = LUST; i != DEFAULT; i++){
		if(chance < town[person].personality[i])
			return i;
		chance = chance - town[person].personality[i];
	}
	return DEFAULT;

}

int setGoal(NPClite* town, int npc2, int prevAction){
	switch(prevAction){
		case FLIRT:{
			int result = roll(town,npc2,HL);
			if(result == DISHONESTY)
				return ASKFORDISTANCE;
			if(result == LUST)
				return SEX;
			break;
			}
		case DATE:{
			int result = roll(town,npc2,DISLOYALTY);
			if(result == DISLOYALTY)
				return BREAKUP;
			return DATE;
			break;
			}
		case SEX:{
			int result = roll(town,npc2,DISLOYALTY);
			if(result == DEFAULT)
				return DATE;
			break;
			}
		case ENVY:{
			int result = roll(town,npc2,WRATH);
			if(result == DEFAULT)
				return ASKFORDISTANCE;
			return VERBALFIGHT;
			break;
			}
		case ROB:{
			int result = roll(town,npc2,WRATH);
			if(result == DEFAULT)
				return VERBALFIGHT;
			return PHYSICALFIGHT;
			break;
			}
		case VERBALFIGHT:{
			int result = roll(town,npc2,WRATH);
			if(result == WRATH)
				return PHYSICALFIGHT;
			break;
			}
		case PHYSICALFIGHT:{
			break;
			}
		case LIE:{
			break;
			}
		case ASKFORDISTANCE:{
			int result = roll(town,npc2,WRATH);
			if(result == WRATH)
				return VERBALFIGHT;
			}
		case SOCIALIZE:{
			break;
			}
		case BREAKUP:{
			int result = roll(town,npc2,WRATH);
			if(result == WRATH){
				return VERBALFIGHT;
			}
			}
	}
	return -1;
}

void chooseAction(NPClite* town, int firstNPC, int secondNPC){
	if(!hasDoneEventMutual(town,INTRODUCE,firstNPC,secondNPC)){
		introduce(town,firstNPC,secondNPC);
		return;
	}
	int trait = -1;
	int lastEvent = getLastInteraction(town,firstNPC,secondNPC)->event;
	switch(lastEvent){
		case FLIRT:
			
			trait = roll(town,firstNPC,HL);
			if(getLastInteraction(town,firstNPC,secondNPC)->npcName1.compare(town[firstNPC].name) != 0 && trait == DISHONESTY){	
				askForDistance(town,firstNPC,secondNPC);
				return;				
			}
			if(trait == LUST && (!isDating(town,firstNPC) || town[firstNPC].personality[DISLOYALTY] > LOVE) && (!isDating(town,secondNPC) || town[secondNPC].personality[DISLOYALTY] > LOVE)){
				sex(town,firstNPC,secondNPC);
				return;
			}
			if(trait == DEFAULT && !isDating(town,firstNPC) && !isDating(town,secondNPC)){
				date(town,firstNPC,secondNPC);
				return;
			}
			else{
				break;
			}
		case DATE:
			trait = roll(town,firstNPC,DISLOYALTY);
			if(trait == DISLOYALTY){
				breakUp(town,firstNPC,secondNPC);
				return;
			}else{
				date(town,firstNPC,secondNPC);
				return;
			}
		case SEX:
			break;
		case ENVY:
			if(getLastInteraction(town,firstNPC,secondNPC)->npcName1.compare(town[firstNPC].name) != 0){
				askForDistance(town,firstNPC,secondNPC);
				return;
			}
			trait = roll(town,firstNPC,GREED);
			if(trait == GREED){
				rob(town,firstNPC,secondNPC);
				return;
			}
			break;
		case ROB:
			if(getLastInteraction(town,firstNPC,secondNPC)->npcName1.compare(town[firstNPC].name) != 0){
				verbalFight(town,firstNPC,secondNPC);
				return;
			}
		case VERBALFIGHT:
			trait = roll(town,firstNPC,WRATH);
			if(trait == WRATH){
				physicalFight(town,firstNPC,secondNPC);
				return;
			}
			break;
		case PHYSICALFIGHT:
			break;
		case LIE:
			break;
		case ASKFORDISTANCE:
			break;
		case SOCIALIZE:
			trait = roll(town,firstNPC,DEFAULT);
			if(trait == LUST){
				flirt(town,firstNPC,secondNPC);
				return;
			}
			if(trait == GREED){
				envy(town,firstNPC,secondNPC);
				return;
			}
			if(trait == WRATH){
				verbalFight(town,firstNPC,secondNPC);
				return;
			}
			if(trait == DISHONESTY){
				lie(town,firstNPC,secondNPC);
				return;
			}
				break;
		}
	socialize(town,firstNPC,secondNPC);
}

void swap(int* current, int a, int b, int currentNoInteract){

	if(currentNoInteract > 1){
		if(a < currentNoInteract && b < currentNoInteract){
			current[a] = current[currentNoInteract];
			current[b] = current[currentNoInteract + 1];
		}
		if(a >= currentNoInteract && b < currentNoInteract){
			for(int i = currentNoInteract; i < currentNoInteract + 2; i++){
				if(i != a)
					current[b] = current[i];
			}
		}
		if(b >= currentNoInteract && a < currentNoInteract){
			for(int i = currentNoInteract; i < currentNoInteract + 2; i++){
				if(i != b)
					current[a] = current[i];
			}
		}
	}
}

void doGoal(NPClite* town, int firstNPC, int secondNPC, int goal){
	switch(goal){
		case FLIRT:
			flirt(town,firstNPC,secondNPC);
		case DATE:
			date(town,firstNPC,secondNPC);
		case SEX:
			sex(town,firstNPC,secondNPC);
		case ENVY:
			envy(town,firstNPC,secondNPC);
		case ROB:
			rob(town,firstNPC,secondNPC);
		case VERBALFIGHT:
			verbalFight(town,firstNPC,secondNPC);
		case PHYSICALFIGHT:
			physicalFight(town,firstNPC,secondNPC);
		case LIE:
			lie(town,firstNPC,secondNPC);
		case ASKFORDISTANCE:
			askForDistance(town,firstNPC,secondNPC);
		case SOCIALIZE:
			socialize(town,firstNPC,secondNPC);
		case BREAKUP:
			breakUp(town,firstNPC,secondNPC);
	}
	town[firstNPC].goal[secondNPC] = -1;
}


void interact(NPClite* town){
	int* towniesByLocation = new int[TOWN_SIZE];
	int* locationPeopleCount = new int[LOCATION];
	int* current = new int[TOWN_SIZE];
	for(int i = 0; i < TOWN_SIZE; i++){
		locationPeopleCount[town[i].schedule[CLOCK%12]]++;
		current[i] = TOWN_SIZE;
	}
	for(int i = 0; i < TOWN_SIZE; i++){
		towniesByLocation[i] = town[i].schedule[CLOCK%12];
	}	
	if(DEBUG_MODE)
		std::cout << "\n";
	for(int i = 0; i < LOCATION; i++){
		int numberInLocation = 0;
		for(int j = 0; j < TOWN_SIZE; j++){
			if(towniesByLocation[j] == i){
				current[numberInLocation++] = j;
			}
		}
		while(numberInLocation > 1){
			int goal = -1;
			int goalTarget = -1;
			int a = z_rand() % numberInLocation;
			for(int i = 0; i < TOWN_SIZE; i++){
				if(town[a].goal[i] > goal && sameLocation(town,a,i)){
					goal = town[a].goal[i];
					goalTarget = i;
				}
			}
			if(goalTarget > -1){
				doGoal(town,a,goalTarget,goal);
				town[goalTarget].goal[a] = setGoal(town, goalTarget, goal);
				numberInLocation = numberInLocation - 2;
				swap(current,a,goalTarget,numberInLocation);
			}else{
				int b = z_rand() % numberInLocation;
				while(a == b)
					b = z_rand() % numberInLocation;
				int lastEvent = -1;
				chooseAction(town, current[a], current[b]);
				if(hasDoneEventMutual(town,INTRODUCE,a,b))
					int lastEvent = getLastInteraction(town,a,b)->event;
				if(lastEvent > 0)				
					setGoal(town,b,lastEvent);
				numberInLocation = numberInLocation - 2;
				swap(current,a,b,numberInLocation);
			}
		}
	}
}

void tick(NPClite* town){
	interact(town);
	CLOCK++;
}
bool didMurder(NPClite* town){
	for(int i = 0; i < TOWN_SIZE; i++){
		for(int j = 0; j < TOWN_SIZE; j++){
				if(town[i].murderiness - 100 > town[i].relationships[j] && i != j){
					if(DEBUG_MODE){
						std::cout << "\n";
						std::cout << town[i].name << " has murdered " << town[j].name << "\n";
				}
					town[i].didMurder = true;
					town[j].isDead = true;
					makeUpCoverUp(town,i,j);
					
					if(MURDER_MODE){
					std::cout << town[i].name << " is the murderer. \n \n\n\n\n\n\n\n\n\n\n\n\n";
					std::cout << town[j].name << " is dead. \n";
					}
				
				return true;
			}
		}
	}
	return false;
}

void simulation(NPClite* town, unsigned int givenSeed) {
	CLOCK = 0;
	seed = givenSeed;
	if(SEED)
		std::cout << "Seed: " << givenSeed << "\n";

	bool murder = false;

	while(!murder){
		if(CLOCK%4 == 0){
			wakeUp(town);
			}
		tick(town);
		if(didMurder(town))
				break;
		if(CLOCK == 200){
			std::cout<<"\nThe members of the town are too kind. No one was murdered. \n";
			murder = true;
		}
	}
	if(EVENT_MODE){
		for(int i = 0; i < TOWN_SIZE; i++){
			for(int j = 0; j < town[i].memories.getSize(); j++)
				printEvent(town[i].memories.getMemory(j));
		std::cout << "\n";
		}
	}
	if(HEARSAY_MODE){
		for(int i = 0; i < TOWN_SIZE; i++){
			for(int j = 0; j < town[i].hearSay.getSize(); j++)
				printEvent(town[i].hearSay.getMemory(j));
		std::cout << "\n";
		}
	}
	if(OBSERVATION_MODE){
		for(int i = 0; i < TOWN_SIZE; i++){
			for(int j = 0; j < town[i].observations.getSize(); j++)
				printEvent(town[i].observations.getMemory(j));
		std::cout << "\n";
		}
	}
}
