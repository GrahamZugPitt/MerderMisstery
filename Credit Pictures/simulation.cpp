#include "event.cpp"
#include <vector>
#include "bmp_edit.hpp"
#include "npc.hpp"
#include "EventArray.cpp"
#include <cstdlib>
#include <ctime>

const int DEBUG_MODE = 1;
const int TOWN_SIZE = 12;
const int LOCATION = 5;
int CLOCK = 0; //"Global variables are bad practice." - Losers, probably

enum observer{
	SUBJECT,
	FIRSTHAND,
	SECONDHAND
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
};
enum event{
	FLIRT,
	INTRODUCE,
	DATE,
	MARRY,
	BREAKUP,
	OWO,
	SOCIALIZE,
};

//Stand in for NPCs. Need to talk to Erick about replacing the NPC memory sets with this because
//I couldn't figure out how the code worked by myself (which is my fault, not his)

class NPClite{

public:
	std::string name;
	int mood;
	EventArray memories;
	int relationships[TOWN_SIZE];
	int personality[5];
	int schedule[12];
	int murderiness;
	int role;
	
	NPClite(std::string name, int role){ //constructor
		srand(time(NULL));
		this->name = name;
		this->role = role;
		for(int i = 0; i < TOWN_SIZE; i++)
			relationships[i] = 0;
		for(int i = 0; i < 5; i++)
			personality[i] = (rand() % 201) /*-100 */;
		murderiness = 0;
		mood = 0;
		
		switch(role){ //assign schedules
			case MAYOR:
				schedule[0] = TOWN_HALL;
				schedule[1] = MARKET;
				schedule[2] = CHURCH;
				schedule[3] = MARKET;
				schedule[4] = TOWN_HALL;
				schedule[5] = MARKET;
				schedule[6] = CHURCH;
				schedule[7] = MARKET;
				schedule[8] = CHURCH;
				schedule[9] = MARKET;
				schedule[10] = TOWN_HALL;
				schedule[11] = MARKET;
				break;
			case PRIEST:
				schedule[0] = FACTORY;
				schedule[1] = MARKET;
				schedule[2] = FACTORY;
				schedule[3] = MARKET;
				schedule[4] = INN;
				schedule[5] = MARKET;
				schedule[6] = CHURCH;
				schedule[7] = MARKET;
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
				schedule[4] = FACTORY;
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
				schedule[3] = MARKET;
				schedule[4] = MARKET;
				schedule[5] = MARKET;
				schedule[6] = INN;
				schedule[7] = MARKET;
				schedule[8] = CHURCH;
				schedule[9] = MARKET;
				schedule[10] = TOWN_HALL;
				schedule[11] = MARKET;
				break;
			case POLICE:
				schedule[0] = TOWN_HALL;
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
				schedule[0] = INN;
				schedule[1] = MARKET;
				schedule[2] = INN;
				schedule[3] = INN;
				schedule[4] = INN;
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
		case FLIRT:
			return "flirted with";
		case INTRODUCE:
			return "met";	
		case DATE:
			return "began dating";
		case MARRY:
			return "married";
		case BREAKUP:
			return "broke up with";
		case OWO:
			return "walked off to a private place holding hands with";
		case SOCIALIZE:
			return "socialized with";
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
void printEvent(Event* e){
	std::cout << e->npcName1 << " " << eventConverter(e->event) << " " << e->npcName2 << " at time " << CLOCK << " at " << locationConverter(e->location) << ". \n";
}
//checks if NPC one has done an event to NPC two (Sometimes, this gurantees that they have both done the event to each other).
bool hasDoneEvent(NPClite* town, int event, int firstNPC, int secondNPC){
	for(int i = 0; i < town[firstNPC].memories.getSize(); i++){
		if (!town[firstNPC].memories.getMemory(i)->npcName2.compare(town[secondNPC].name) && town[firstNPC].memories.getMemory(i)->event == event)
			return true;
	}
	return false;
}




void changeRelationship(NPClite* town, int firstNPC, int secondNPC, int deltaRelationship){
	town[firstNPC].relationships[secondNPC] = town[firstNPC].relationships[secondNPC] + deltaRelationship;
	if(town[firstNPC].relationships[secondNPC] > 100)
		town[firstNPC].relationships[secondNPC] = 100;
	if(town[firstNPC].relationships[secondNPC] < -100)
		town[firstNPC].relationships[secondNPC] = -100;
}

void changeMood(NPClite* town, int person, int deltaMood){
	town[person].mood = town[person].mood + deltaMood;
}
//changes an observers memory
void observe(NPClite* town, Event* e, int observer, int firstNPC, int secondNPC, int deltaFirst, int deltaSecond, int deltaMood){
	Event* e2 = new Event(e, FIRSTHAND);
	town[observer].memories.add(e2);
	changeMood(town, observer, deltaMood);
	changeRelationship(town,firstNPC,secondNPC,deltaFirst);
	changeRelationship(town,secondNPC,firstNPC,deltaSecond);
}
//checks to see if someone observed an event first hand
bool didObserve(NPClite* town,  int observer, int firstNPC, int secondNPC){
	return (town[firstNPC].schedule[CLOCK%12] == town[observer].schedule[CLOCK%12] && firstNPC != observer && secondNPC != observer);
}
//adds memory too people experienced first hand
void addMemory(NPClite* town, Event* e, int firstNPC, int secondNPC, int deltaFirst, int deltaSecond, int deltaMoodFirst, int deltaMoodSecond){	
	town[firstNPC].memories.add(e);
	town[secondNPC].memories.add(e);
	//change relationships
	changeRelationship(town,firstNPC,secondNPC,deltaFirst);
	changeRelationship(town,secondNPC,firstNPC,deltaSecond);
	//change moods
	changeMood(town,firstNPC,deltaMoodFirst);
	changeMood(town,secondNPC,deltaMoodSecond);
}
void introduce(NPClite* town, int firstNPC, int secondNPC){
	//create event
	Event* e = new Event(town[firstNPC].name, town[secondNPC].name, INTRODUCE, CLOCK, town[firstNPC].schedule[CLOCK%12], SUBJECT);
	//add memory
	addMemory(town,e,firstNPC,secondNPC,town[secondNPC].mood,town[firstNPC].mood,town[secondNPC].mood,town[firstNPC].mood);
	for(int i = 0; i < TOWN_SIZE; i++){
		if(didObserve(town,i,firstNPC,secondNPC))
			observe(town,e,i,firstNPC,secondNPC,0,0,0);
		}
	if(DEBUG_MODE)
		printEvent(e);
}
//The flirting event 
void flirt(NPClite* town, int firstNPC, int secondNPC){
	//create event
	Event* e = new Event(town[firstNPC].name, town[secondNPC].name, FLIRT, CLOCK, town[firstNPC].schedule[CLOCK%12], SUBJECT);
	//add memory
	addMemory(town, e, firstNPC, secondNPC,5,5,1,1);
	//town observes
	for(int i = 0; i < TOWN_SIZE; i++){
		if(didObserve(town,i,firstNPC,secondNPC)){
			if(hasDoneEvent(town,DATE,i,firstNPC))
				observe(town,e,i,firstNPC,secondNPC,-5,-10,-3);
			else if(hasDoneEvent(town,DATE,i,secondNPC))
				observe(town,e,i,firstNPC,secondNPC,-10,-5,-3); 
			else if(hasDoneEvent(town,MARRY,i,secondNPC))
				observe(town,e,i,firstNPC,secondNPC,-20,-10,-6);
			else if(hasDoneEvent(town,MARRY,i,firstNPC))
				observe(town,e,i,firstNPC,secondNPC,-10,-20,-6); 
			else{observe(town,e,i,firstNPC,secondNPC,0,0,0);}
		}
	//print to console	
	}
	if(DEBUG_MODE)
		printEvent(e);
}
//the dating event
void date(NPClite* town, int firstNPC, int secondNPC){
	//create event
	Event* e = new Event(town[firstNPC].name, town[secondNPC].name, DATE, CLOCK, town[firstNPC].schedule[CLOCK%12], SUBJECT);
	//add memory
	addMemory(town, e, firstNPC, secondNPC,10,10,2,2);
	//town observes
	for(int i = 0; i < TOWN_SIZE; i++){
		if(didObserve(town,i,firstNPC,secondNPC)){
			if(hasDoneEvent(town,DATE,i,firstNPC) && i!= firstNPC)
				observe(town,e,i,firstNPC,secondNPC,-15,-30,-4);
			else if(hasDoneEvent(town,DATE,i,secondNPC))
				observe(town,e,i,firstNPC,secondNPC,-30,-15,-4); 
			else if(hasDoneEvent(town,FLIRT,i,secondNPC))
				observe(town,e,i,firstNPC,secondNPC,-10,-5,-3);
			else if(hasDoneEvent(town,FLIRT,i,firstNPC))
				observe(town,e,i,firstNPC,secondNPC,-5,-10,-3); 
			else{observe(town,e,i,firstNPC,secondNPC,0,0,1);}
		}
	//print to console	
	}
	if(DEBUG_MODE)
		printEvent(e);
}
//the break up event
void breakUp(NPClite* town, int firstNPC, int secondNPC){
	//create event
	Event* e = new Event(town[firstNPC].name, town[secondNPC].name, BREAKUP, CLOCK, town[firstNPC].schedule[CLOCK%12], SUBJECT);
	//add memory
	addMemory(town, e, firstNPC, secondNPC,-25,-25,-3,-3);
	//town observes
	for(int i = 0; i < TOWN_SIZE; i++){
		if(didObserve(town,i,firstNPC,secondNPC)){
			if(hasDoneEvent(town,FLIRT,i,secondNPC) && i!= firstNPC)
				observe(town,e,i,firstNPC,secondNPC,10,5,3);
			else if(hasDoneEvent(town,FLIRT,i,firstNPC) && i!= secondNPC)
				observe(town,e,i,firstNPC,secondNPC,5,10,3); 
			else{observe(town,e,i,firstNPC,secondNPC,0,0,-1);}
		}
	//print to console	
	}
	if(DEBUG_MODE)
		printEvent(e);
}
//OwO what's this?
void sex(NPClite* town, int firstNPC, int secondNPC){
	//create event
	Event* e = new Event(town[firstNPC].name, town[secondNPC].name, OWO, CLOCK, town[firstNPC].schedule[CLOCK%12], SUBJECT);
	//add memory
	addMemory(town, e, firstNPC, secondNPC,50,50,50,50);
	//town observes
	for(int i = 0; i < TOWN_SIZE; i++){
		if(/*didObserve(town,i,firstNPC,secondNPC)*/ true){
			if(hasDoneEvent(town,OWO,i,secondNPC) && i!= firstNPC)
				observe(town,e,i,firstNPC,secondNPC,-50,-50,-50);
			else if(hasDoneEvent(town,MARRY,i,secondNPC) && i!= secondNPC)
				observe(town,e,i,firstNPC,secondNPC,-80,-80,-10); 
			else{observe(town,e,i,firstNPC,secondNPC,0,0,0);}
		}
	//print to console	
	}
	if(DEBUG_MODE)
		printEvent(e);
}
//the marriage event
void marry(NPClite* town, int firstNPC, int secondNPC){
	//create event
	Event* e = new Event(town[firstNPC].name, town[secondNPC].name, MARRY, CLOCK, town[firstNPC].schedule[CLOCK%12], SUBJECT);
	//add memory
	addMemory(town, e, firstNPC, secondNPC,25,25,15,15);
	//town observes
	for(int i = 0; i < TOWN_SIZE; i++){
		if(didObserve(town,i,firstNPC,secondNPC)){
			if(hasDoneEvent(town,DATE,i,secondNPC))
				observe(town,e,i,firstNPC,secondNPC,-10,-10,-1); 
			else{observe(town,e,i,firstNPC,secondNPC,0,0,2);}
		}
	//print to console	
	}
	if(DEBUG_MODE)
		printEvent(e);
}
//the socialize event 
void socialize(NPClite* town, int firstNPC, int secondNPC){
	//create event
	Event* e = new Event(town[firstNPC].name, town[secondNPC].name, SOCIALIZE, CLOCK, town[firstNPC].schedule[CLOCK%12], SUBJECT);
	//get whether mood is positive or negative
	int moodParody1;
	int moodParody2;
	if(town[firstNPC].mood != 0)
		moodParody1 = abs(town[firstNPC].mood)/town[firstNPC].mood;
	else{moodParody1 = 0;}
	if(town[secondNPC].mood != 0)
		moodParody2 = abs(town[secondNPC].mood)/town[secondNPC].mood;
	else{moodParody2 = 0;}
	
	//add memory
	addMemory(town,e,firstNPC,secondNPC,2,2,3*moodParody1,3*moodParody2);
	//town observes
	for(int i = 0; i < TOWN_SIZE; i++){
		if(didObserve(town,i,firstNPC,secondNPC)){
			observe(town,e,i,firstNPC,secondNPC,0,0,0);
		}
	//print to console	
	}
	if(DEBUG_MODE)
		printEvent(e);
}


void wakeUp(NPClite* town){
	std::cout << "DAY " << (CLOCK + 1)/4 << ":\n";
	for(int i = 0; i < TOWN_SIZE; i++){
		if(town[i].mood < 0)
			town[i].murderiness = town[i].murderiness + town[i].mood;
		if(town[i].murderiness = town[i].murderiness < -100)
			town[i].murderiness = -100; //I am intentionally putting no upper bound on murderiness
		town[i].mood = (rand() % 5) - 2;
	}
}
void chooseAction(NPClite* town, int firstNPC, int secondNPC){
	//std::cout << firstNPC << " " << secondNPC;
	if(!hasDoneEvent(town,INTRODUCE,firstNPC,secondNPC))
		introduce(town,firstNPC,secondNPC);
	
	else if (hasDoneEvent(town,OWO,firstNPC,secondNPC)
		&& hasDoneEvent(town,DATE,firstNPC,secondNPC)
		&& !hasDoneEvent(town,BREAKUP,firstNPC,secondNPC)
		&& town[firstNPC].relationships[secondNPC] > 0 
		&& town[secondNPC].relationships[firstNPC] > 0)
		 	marry(town,firstNPC,secondNPC);

	/*else if (town[firstNPC].personality[LUST] > (10 + town[firstNPC].relationships[secondNPC]) 
		&& town[secondNPC].personality[LUST] > (10 + town[firstNPC].relationships[firstNPC]))
			sex(town,firstNPC,secondNPC);*/

	else if (hasDoneEvent(town,DATE,firstNPC,secondNPC)
		&& town[firstNPC].relationships[secondNPC] > 0 
		&& town[secondNPC].relationships[firstNPC] > 0)
		 	sex(town,firstNPC,secondNPC);

	else if (hasDoneEvent(town,FLIRT,firstNPC,secondNPC)
		&& !hasDoneEvent(town,DATE,firstNPC,secondNPC)
		&& town[firstNPC].relationships[secondNPC] > 25
		&& town[secondNPC].relationships[firstNPC] > 25)
		 	date(town,firstNPC,secondNPC);

	else if (/*town[firstNPC].personality[LUST] > 0 
		&& town[secondNPC].personality[LUST] > 0 
		&& town[firstNPC].relationships[secondNPC] > 3 
		&& */town[secondNPC].relationships[firstNPC] > 3)
			flirt(town,firstNPC,secondNPC);
	
	else socialize(town,firstNPC,secondNPC);
}
//I'm pretty sure this code is attrocious but it works so uh... I'll come back and fix it if I have time
void interact(NPClite* town){int a = 0; int b = 0; while(a==b){a = rand()%12; b = rand()%12;} chooseAction(town,a,b);}
	/*int* towniesInOneLocation = new int[TOWN_SIZE];
	int* locationPeopleCount = new int[LOCATION]; 
	for(int i = 0; i < TOWN_SIZE; i++)
		locationPeopleCount[town[i].schedule[CLOCK%12]]++; //counts people in each location
	for(int i = 0; i < LOCATION; i++){ //For each location, begins pairing people up
		//std::cout<<"I GET HERE \n";
		int k = 0;
		for(int j = 0; j < TOWN_SIZE; j++){
			if(town[j].schedule[CLOCK%12] == i){
				towniesInOneLocation[k] = j;
				k++;			
			}
		}
		for(int f = 0; f < locationPeopleCount[i]/2; f++){ 
			int firstNPC = 0;
			int secondNPC = 0; 
			while(firstNPC == secondNPC || towniesInOneLocation[firstNPC] == -1 || towniesInOneLocation[secondNPC] == -1){
				firstNPC = (rand() % locationPeopleCount[i]);
				secondNPC = (rand() % locationPeopleCount[i]);
			}
		chooseAction(town, towniesInOneLocation[firstNPC], towniesInOneLocation[secondNPC]);
		towniesInOneLocation[firstNPC] == -1;
		towniesInOneLocation[secondNPC] == -1;		
		}
	}
} */

void tick(NPClite* town){
	interact(town);
	CLOCK++;	
}
bool didMurder(NPClite* town){
	for(int i = 0; i < TOWN_SIZE; i++){
		for(int j = 0; j < TOWN_SIZE; j++){
				if(town[i].murderiness -50 > town[i].relationships[j]){
					std::cout << town[i].name << " has murdered " << town[j].name << "\n";
				return true;
			}
		}
	}
return false;
}
int main() {
	srand(time(NULL)); //seeds ramdomizer
	//townies
	NPClite jill("Jill Stein", MAYOR); 
	NPClite kim("Kim Cardasian", POLICE);
	NPClite pope("Pope Benedict IX", PRIEST); 
	NPClite gaben("Gabe Newall", INNKEEPER); 
	NPClite marie("Marie Curie", MERCHANT); 
	NPClite lary("Lary the Qcumber", MERCHANT);
	NPClite luigi("Luigi", MERCHANT);
	NPClite albert("Albert Einstein", MERCHANT); 
	NPClite dennis("Dennis Preger", WORKER); 
	NPClite helen("Helen of Troy", WORKER); 
	NPClite merge("Marge Simpzon", WORKER);
	NPClite sigmund("Sigmund Frued", WORKER);
	NPClite town[TOWN_SIZE] = {jill, kim, pope, gaben, marie, lary, luigi, albert, dennis, helen, merge, sigmund};
	
	bool murder = false;	
	
	while(!murder){
		if(CLOCK%4 == 0){
			wakeUp(town);
			if(didMurder(town))
				break;
			}
		tick(town);
		if(CLOCK == 800){
			std::cout<<"\nThe members of the town are too kind. No one was murdered. \n";
			murder = true;
		}
	}
	return 0;
}
