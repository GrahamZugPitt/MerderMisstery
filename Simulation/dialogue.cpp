#include "simulationWithGoals.cpp"

const int DIALOGUE_OPTION_COUNT = 40;
const int NUMBER_OF_STATES = 10;
const int DIALOGUE_OPTIONS_PER_STATE = 4;
const int CHOOSE_NPC_MODE = 1;


enum dishonesty{
	LITERALLY_CANNOT_LIE = 5,
	SOMEWHAT_DISHONEST = 10,
	VERY_DISHONEST = 15,
};

enum stateOptions{
	INITIAL_STATE,
	NPCS1,
	NPCS2,
	NPCS3,
	NPCS4,
	ACTION1,
	ACTION2,
	ACTION3,
	ACTION4,
	SELF_OR_OTHERS,
};

enum dialogueOptions{
	NPC0,
	NPC1,
	NPC2,
	NPC3,
	NPC4,
	NPC5,
	NPC6,
	NPC7,
	NPC8,
	NPC9,
	NPC10,
	NPC11,
	HOWFEEL,
	WHENSEEN,
	HEARD,
	DONEWITH,
	WHATLIKE,
	MOSTRECENT,
	FILLER,
};

class dialogueOption{
	public:
		int nextTag;
		int tag;
		std::string option;

	dialogueOption(){
		nextTag = -1;
		tag = -1;
		option = "Graham programmed me wrong - so now I am saying this.";
	}

	dialogueOption(std::string words, int identifier, int nextIdentifier){
		nextTag = nextIdentifier;
		tag = identifier;
		option = words;
	}
	~dialogueOption();
};

class dialogueOptionList{
	public:
		dialogueOption** options;
	
	dialogueOptionList(){
		options = new dialogueOption*[DIALOGUE_OPTIONS_PER_STATE];
		options[0] = new dialogueOption("Graham programmed me wrong - so now I am saying this.",-1,-1);
		options[1] = new dialogueOption("Graham programmed me wrong - so now I am saying this.",-1,-1);
		options[2] = new dialogueOption("Graham programmed me wrong - so now I am saying this.",-1,-1);
		options[3] = new dialogueOption("Graham programmed me wrong - so now I am saying this.",-1,-1);
	}
	
	dialogueOptionList(dialogueOption* zero, dialogueOption* one, dialogueOption* two, dialogueOption* three){ //constructor
		options = new dialogueOption*[DIALOGUE_OPTIONS_PER_STATE];
		options[0] = zero;
		options[1] = one;
		options[2] = two;
		options[3] = three;
}
	dialogueOption getOption(int choice){
		return *options[choice];
	}
	
	void printState(){
		for(int i = 0; i < DIALOGUE_OPTIONS_PER_STATE; i++)
			std::cout << i << ") " << options[i]->option << "\n"; //TODO: This is the function that prints out each dialogue option for a given state. Change this so that options[i] is blitted to the ith dialogue rectangle. 
	}
	

};


int getNPC(NPClite* town){
	for(int i = 0; i < TOWN_SIZE; i++){
		if(town[i].isDead)
			return i;
	}
}

dialogueOptionList* initializeOptions(NPClite* town){
	std::string dead = town[getNPC(town)].name;
	dialogueOption* o0 = new dialogueOption("Who dislikes " + dead + "?", HOWFEEL, NPCS1);
	dialogueOption* o1 = new dialogueOption("When was the last time you saw " + dead + "?", WHENSEEN, NPCS1);
	dialogueOption* o2 = new dialogueOption("What have you gossip have you heard?",HEARD, NPCS1);
	dialogueOption* o3= new dialogueOption("What have you...?", DONEWITH, ACTION1);
	
	dialogueOption* t0 = new dialogueOption(town[0].name, NPC0, INITIAL_STATE);
	dialogueOption* t1 = new dialogueOption(town[1].name, NPC1, INITIAL_STATE);
	dialogueOption* t2 = new dialogueOption(town[2].name, NPC2, INITIAL_STATE);
	dialogueOption* t3 = new dialogueOption(town[3].name, NPC3, INITIAL_STATE);
	dialogueOption* t4 = new dialogueOption(town[4].name, NPC4, INITIAL_STATE);
	dialogueOption* t5 = new dialogueOption(town[5].name, NPC5, INITIAL_STATE);
	dialogueOption* t6 = new dialogueOption(town[6].name, NPC6, INITIAL_STATE);
	dialogueOption* t7 = new dialogueOption(town[7].name, NPC7, INITIAL_STATE);
	dialogueOption* t8 = new dialogueOption(town[8].name, NPC8, INITIAL_STATE);
	dialogueOption* t9 = new dialogueOption(town[9].name, NPC9, INITIAL_STATE);
	dialogueOption* t10 = new dialogueOption(town[10].name, NPC10, INITIAL_STATE);
	dialogueOption* t11 = new dialogueOption(town[11].name, NPC11, INITIAL_STATE);

	dialogueOption* a0 = new dialogueOption("Asked for distance from?", ASKFORDISTANCE, NPCS1);
	dialogueOption* a1 = new dialogueOption("Flirted with?", FLIRT, NPCS1);
	dialogueOption* a2 = new dialogueOption("Dated?", DATE, NPCS1);
	dialogueOption* a3 = new dialogueOption("Done things you can't do in CS1666 to?", SEX, NPCS1);
	dialogueOption* a4 = new dialogueOption("Broken up with?", BREAKUP, NPCS1);
	dialogueOption* a5 = new dialogueOption("Looked at, as if plotting theft?", ENVY, NPCS1);
	dialogueOption* a6 = new dialogueOption("Robbed?", ROB, NPCS1);
	dialogueOption* a7 = new dialogueOption("Verbally fought?", VERBALFIGHT, NPCS1);
	dialogueOption* a8 = new dialogueOption("Physically fought?", PHYSICALFIGHT, NPCS1);
	
	
	dialogueOption* nNPC1 = new dialogueOption(town[0].name + ", " + town[1].name + ", " + town[2].name + ", "+ town[3].name, FILLER, NPCS2);
	dialogueOption* nNPC2 = new dialogueOption(town[4].name + ", " + town[5].name+ ", " + town[6].name+ ", " + town[7].name, FILLER, NPCS3);
	dialogueOption* nNPC3 = new dialogueOption(town[8].name + ", " + town[9].name+ ", " + town[10].name + ", "+ town[11].name, FILLER, NPCS4);

	dialogueOption* nA1 = new dialogueOption("Asked for Distance? Flirted? Dated?", FILLER, ACTION2);
	dialogueOption* nA2 = new dialogueOption("Things you can't in CS1666? Broken up? Gazed at?", FILLER, ACTION3);
	dialogueOption* nA3 = new dialogueOption("Robbed? Verbally fought? Physically fought?", FILLER, ACTION4);

	dialogueOption* self = new dialogueOption("done with " + dead + "?", FILLER, NPCS1);
	dialogueOption* others = new dialogueOption("seen " + dead + " do with others?", FILLER, NPCS1);	

	dialogueOption* blank = new dialogueOption("", FILLER, NPCS1);
	dialogueOption* blank2 = new dialogueOption("", FILLER, ACTION1);
	dialogueOption* blank3 = new dialogueOption("", FILLER, ACTION2);
	dialogueOption* blank4 = new dialogueOption("", FILLER, ACTION3);
	dialogueOption* blank5 = new dialogueOption("", FILLER, ACTION4);
	dialogueOption* blank6 = new dialogueOption("", FILLER, SELF_OR_OTHERS);
	
	
	dialogueOption** options = new dialogueOption*[DIALOGUE_OPTION_COUNT];

	options[0] = o0;
	options[1] = o1;
	options[2] = o2;
	options[3] = o3;

	options[4] = nNPC1;
	options[5] = nNPC2;
	options[6] = nNPC3;
	options[7] = blank;

	options[8] = t0;
	options[9] = t1;
	options[10] = t2;
	options[11] = t3;
	options[12] = t4;
	options[13] = t5;
	options[14] = t6;
	options[15] = t7;	
	options[16] = t8;
	options[17] = t9;
	options[18] = t10;
	options[19] = t11;

	options[20] = nA1;
	options[21] = nA2;
	options[22] = nA3;
	options[23] = blank2;

	options[24] = a0;
	options[25] = a1;
	options[26] = a2;
	options[27] = blank3;

	options[28] = a3;
	options[29] = a4;
	options[30] = a5;
	options[31] = blank4;

	options[32] = a6;
	options[33] = a7;
	options[34] = a8;
	options[35] = blank5;
		
	options[36] = others;
	options[37] = self; //order here matter
	options[38] = blank6;
	options[39] = blank6;
	
	dialogueOptionList* globalList = new dialogueOptionList[NUMBER_OF_STATES];
	
	for(int i = 0; i < NUMBER_OF_STATES; i++)	
		globalList[i] = dialogueOptionList(options[i*DIALOGUE_OPTIONS_PER_STATE],options[i*DIALOGUE_OPTIONS_PER_STATE+1],options[i*DIALOGUE_OPTIONS_PER_STATE+2],options[i*DIALOGUE_OPTIONS_PER_STATE+3]);
	
	return globalList;
}
std::string eventConverterDialogue(int event){
	switch(event){
		case CDATE:
			return "was being romantic with";
		case FLIRT:
			return "was flirting with";
		case INTRODUCE:
			return "was getting to know";
		case DATE:
			return "started dating";
		case BREAKUP:
			return "broke up with";
		case SEX:
			return "was doing things you can't talk about in CS1666 with";
		case SOCIALIZE:
			return "was chatting with";
		case ENVY:
			return "was glaring with eyes of greed at";
		case ROB:
			return "robbed";
		case VERBALFIGHT:
			return "got into a shouting match with";
		case PHYSICALFIGHT:
			return "was throwing hands with";
		case LIE:
			return "was telling a story to";
		case ASKFORDISTANCE:
			return "was asking for distance from";
		}
	return "performed an unperformable action on";
}

std::string eventConverterAskEvent(int event){
	switch(event){
		case CDATE:
			return "has been in a committed relationship with";
		case FLIRT:
			return "has flirted with";
		case DATE:
			return "has dated";
		case BREAKUP:
			return "has broken up with";
		case SEX:
			return "has done thing you can't talk about in CS1666 with";
		case ENVY:
			return "has glared with eyes of greed at";
		case ROB:
			return "has been accused of robbery by";
		case VERBALFIGHT:
			return "has gotten into a shouting match with";
		case PHYSICALFIGHT:
			return "thrown hands with";
		case LIE:
			return "has gossiped with";
		case SOCIALIZE:
			return "has gossiped with";
		case ASKFORDISTANCE:
			return "has asked for distance from";
		}
	return "performed an unperformable action on";
}

std::string getTime(int time){
	std::string statement = "";
	int days = (CLOCK - time)/4;
	time = time % 4;
	switch(time){
		case 0:
			statement = "in the morning.";
			break;
		case 1:
			statement = "in the afternoon.";
			break;
		case 2:
			statement = "in the evening.";
			break;
		case 3:
			statement = "at night.";
			break;
		}
	if(days)
		statement = statement + " " + std::to_string(days) + " day(s) ago.";
	return statement;
}

bool compareNames(NPClite* town, int npc1, std::string npc2){
	if(!town[npc1].name.compare(npc2))
		return true;
	return false;
}

void printEventDialogue(Event* e){
			std::cout << e->npcName1 << " " << eventConverterDialogue(e->event) << " " << e->npcName2 << " " << getTime(e->time) << " at " << locationConverter(e->location) << ". \n"; //TODO: Change std::cout to a blit to the screen.	
}

void printEventDialogueFPF(Event* e){
			std::cout << "I " << eventConverterDialogue(e->event) << " " << e->npcName2 << " " << getTime(e->time) << " at " << locationConverter(e->location) << ". \n";	//TODO: Change std::cout to a blit to the screen.	
}
void printEventDialogueFPS(Event* e){
			std::cout << e->npcName1 << " " << eventConverterDialogue(e->event) << " me " << getTime(e->time) << " at " << locationConverter(e->location) << ". \n"; //TODO: Change std::cout to a blit to the screen.		
}

void printEventDialogueGossip(Event* e){
			std::cout << "I heard " << e->npcName1 << " " << eventConverterDialogue(e->event) << " " << e->npcName2 << " " << getTime(e->time) << " at " << locationConverter(e->location) << ". \n"; //TODO: Change std::cout to a blit to the screen.		
}

bool suspeciousEvent(int event){
	if(event == BREAKUP || event == ROB || event == PHYSICALFIGHT) return true;
	return false;
}

bool slightlySuspeciousEvent(int event){
	if(event == ASKFORDISTANCE || event == VERBALFIGHT || event == ENVY) return true;
	return false;
}

bool sayPersonalEventMurderer(NPClite* town, int event, int beingInterrogated){
	
	if(town[beingInterrogated].didMurder)

	if((!suspeciousEvent(event) 
	|| town[beingInterrogated].personality[DISHONESTY] < SOMEWHAT_DISHONEST)
	&& (!slightlySuspeciousEvent(event) 
	|| town[beingInterrogated].personality[DISHONESTY] < VERY_DISHONEST)){
		return true;
	}
	return false;
}

bool sayPersonalEvent(NPClite* town, int event, int beingInterrogated){
	
	if(town[beingInterrogated].didMurder)
		return sayPersonalEventMurderer(town,event,beingInterrogated);
	if((!suspeciousEvent(event) 
	|| town[beingInterrogated].personality[DISHONESTY] < LITERALLY_CANNOT_LIE)
	&& (!slightlySuspeciousEvent(event) 
	|| town[beingInterrogated].personality[DISHONESTY] < SOMEWHAT_DISHONEST)){
		return true;
	}
	return false;
}

void doneWithSelf(NPClite* town, int beingInterrogated, int npc){
	bool anyEvent = false;
	for(int i = 0; i < town[beingInterrogated].memories.getSize(); i++){
		Event* e = town[beingInterrogated].memories.getMemory(i);
		if(compareNames(town,npc,e->npcName1) && e->event != INTRODUCE && e->event != SOCIALIZE && !suspeciousEvent(e->event)){
			printEventDialogueFPS(e);
			anyEvent = true;
			}
		if(compareNames(town,npc,e->npcName2) 
		&& e->event != INTRODUCE 
		&& e->event != SOCIALIZE 
		&& sayPersonalEvent(town, e->event, beingInterrogated)){
			printEventDialogueFPF(e);
			anyEvent = true;
			}
	}
	if(!anyEvent)
		std::cout << "I've only made small talk with " << town[npc].name + "."; //TODO: Change std::cout to a blit to the screen.	
}

void doneWith(NPClite* town, int beingInterrogated, int npc){
	bool anyEvent = false;
	for(int i = 0; i < town[beingInterrogated].observations.getSize(); i++){
		Event* e = town[beingInterrogated].observations.getMemory(i);
		if(compareNames(town,npc,e->npcName1) && e->event != INTRODUCE && e->event != SOCIALIZE){
			printEventDialogue(e);
			anyEvent = true;
			}
		if(compareNames(town,npc,e->npcName2) && e->event != INTRODUCE && e->event != SOCIALIZE){
			printEventDialogue(e);
			anyEvent = true;
			}
	}
	if(!anyEvent)
	std::cout << "I've only ever seen " << town[npc].name + " make small talk with others."; //TODO: Change std::cout to a blit to the screen.	
}
std::string howFeel(NPClite* town, int npc, int beingInterrogated){
	std::string dislike = "";
	bool nobody = true;
	for(int i = 0; i < TOWN_SIZE; i++){
		if(hasDoneEventMutual(town,INTRODUCE,beingInterrogated,i) && town[i].relationships[npc] < 0){			
			if(nobody){
				if(i != beingInterrogated){
					nobody = false;			
					dislike = dislike + town[i].name;
				}else if (!town[npc].isDead || (!town[beingInterrogated].didMurder && town[beingInterrogated].personality[DISHONESTY] < VERY_DISHONEST) || (town[beingInterrogated].didMurder && town[beingInterrogated].personality[DISHONESTY] < SOMEWHAT_DISHONEST)){
					nobody = false;
					dislike = dislike + "I";
				}
			}
			else if(i != beingInterrogated)			
				dislike = dislike + " and " + town[i].name;
			else if (town[beingInterrogated].personality[DISHONESTY] < VERY_DISHONEST){
				dislike = dislike + " and I";
			}
		}
	}
	dislike = dislike + " dislike(s) " + town[npc].name + ".\n";
	if(nobody)
		dislike = "I don't know anyone who dislikes " + town[npc].name + ".\n";
	return dislike;
}

bool eventsAreEqual(Event* e, Event* e2){
	if(!e->npcName1.compare(e2->npcName1) && (!e->npcName2.compare(e2->npcName2)) &&
	(e->event == e2->event)){
		return true;
	}
	return false;
}

void heard(NPClite* town, int beingInterrogated){
	Event* gossip[town[beingInterrogated].hearSay.getSize()];
	int j = 0;
	for(int i = 0; i < town[beingInterrogated].hearSay.getSize(); i++){
		bool alreadyMentioned = false;
		for(int k = 0; k < j; k++){
			if(eventsAreEqual(town[beingInterrogated].hearSay.getMemory(i), gossip[k]))
			alreadyMentioned = true;
		}
		if(!alreadyMentioned && !wasInvolvedInEvent(town,town[beingInterrogated].hearSay.getMemory(i),beingInterrogated)){
			printEventDialogueGossip(town[beingInterrogated].hearSay.getMemory(i));
			gossip[j++] = town[beingInterrogated].hearSay.getMemory(i);
		}
	}
}

std::string whenSeen(NPClite* town, int npc, int beingInterrogated){
	std::string lastMemory = "I can't remember when I last saw " + town[npc].name; //TODO: Change std::cout to a blit to the screen.	
	int event = -1;
	int eventTime = -1;
	Event* e = new Event();
	bool wasObservation = false;
	for(int i = town[beingInterrogated].memories.getSize() - 1; i > -1; i--){
		if(wasInvolvedInEvent(town,town[beingInterrogated].memories.getMemory(i),npc) && 
sayPersonalEvent(town,town[beingInterrogated].memories.getMemory(i)->event,beingInterrogated)){
			event = town[beingInterrogated].memories.getMemory(i)->event;
			eventTime = town[beingInterrogated].memories.getMemory(i)->time;
			e = town[beingInterrogated].memories.getMemory(i);
			break;
		}
	}
	if(town[beingInterrogated].observations.getSize()){
		for(int i = town[beingInterrogated].observations.getSize() - 1; i > -1; i--){
			if(wasInvolvedInEvent(town,town[beingInterrogated].observations.getMemory(i),npc) && town[beingInterrogated].observations.getMemory(i)->time > eventTime){
				event = town[beingInterrogated].observations.getMemory(i)->event;
				eventTime = town[beingInterrogated].observations.getMemory(i)->time;
				e = town[beingInterrogated].observations.getMemory(i);
				wasObservation = true;
				break;
				}
			}
		}
	if(event != -1){
		if(!e->npcName1.compare(town[beingInterrogated].name)){
		lastMemory = "I " + eventConverterDialogue(event) + " " + e->npcName2 + " at " + locationConverter(e->location) + ". That was " + getTime(eventTime);
	return lastMemory;		
	}
		if(!e->npcName2.compare(town[beingInterrogated].name)){
		lastMemory = e->npcName1 + " " + eventConverterDialogue(event) + " me" + " at " + locationConverter(e->location) + ". That was " + getTime(eventTime);
	return lastMemory;	
	}
		lastMemory = e->npcName1 + " " + eventConverterDialogue(event) + " " + e->npcName2 + " at " + locationConverter(e->location) + ". That was " + getTime(eventTime);
	}
	return lastMemory;
}

bool selfOrOthers(dialogueOptionList* globalOptionList){
	int dialogueState = SELF_OR_OTHERS;
	int input = -1;
	globalOptionList[SELF_OR_OTHERS].printState();
	std::cin >> input; //TODO: Replace this with a choice of an interger representation of one of the dialogue boxes. 
	dialogueState = globalOptionList[dialogueState].options[input]->nextTag;
	while(input % 4 == 3 || input % 4 == 2){
		globalOptionList[dialogueState].printState(); //THIS NEEDS TO BE BLIT'D TO THE SCREEN
		std::cin >> input; //TODO: Replace this with a choice of an interger representation of one of the dialogue boxes. 
		dialogueState = globalOptionList[dialogueState].options[input]->nextTag;
		}
	return (input == 1);
}

//This method was going to be the method for getting an NPC of the player's choice to ask about, but in practice, the only information the player needs involves relationships with the dead person.
/*
int getNPC(NPClite* town, dialogueOptionList* globalOptionList, int dialogueState){
	int input = -1;
	globalOptionList[dialogueState].printState(); //THIS NEEDS TO BE BLIT'D TO THE SCREEN
	std::cin >> input;//EDIT THIS IN THE ACTUAL IMPLEMENTATION
	dialogueState = globalOptionList[dialogueState].options[input]->nextTag;
	while(input % 4 == 3){
		globalOptionList[dialogueState].printState(); //THIS NEEDS TO BE BLIT'D TO THE SCREEN
		std::cin >> input;//EDIT THIS IN THE ACTUAL IMPLEMENTATION
		dialogueState = globalOptionList[dialogueState].options[input]->nextTag;
		}
		globalOptionList[dialogueState].printState(); //THIS NEEDS TO BE BLIT'D TO THE SCREEN
		std::cin >> input;//EDIT THIS IN THE ACTUAL IMPLEMENTATION
	
	return globalOptionList[dialogueState].options[input]->tag;
}
*/
//This method was never used, but was kept because it may be used in a future implementation
/*
int getAction(NPClite* town, dialogueOptionList* globalOptionList, int dialogueState){
	int input = -1;
	for(int i = 0; i < 2; i++){
		globalOptionList[dialogueState].printState(); //THIS NEEDS TO BE BLIT'D TO THE SCREEN
		std::cin >> input; 
		dialogueState = globalOptionList[dialogueState].options[input]->nextTag;
		while(input % 4 == 3){
			globalOptionList[dialogueState].printState(); //THIS NEEDS TO BE BLIT'D TO THE SCREEN
			std::cin >> input;//EDIT THIS IN THE ACTUAL IMPLEMENTATION
			dialogueState = globalOptionList[dialogueState].options[input]->nextTag;
			}
		}
	return globalOptionList[dialogueState].options[input]->tag;
} */

bool goodMurder(NPClite* town){
	int suspects = 0;
	int victim = -1;
	for(int i = 0; i < TOWN_SIZE; i++){
		if(town[i].isDead){
			victim = i;
			break;
		}
	}
	if(victim == -1)
		return false;
	for(int i = 0; i < TOWN_SIZE; i++){
		if(town[i].relationships[victim] < 0)
			suspects++;
	}
	if(suspects > 2)
		return true;
	return false;
}



int dialogue(NPClite* town){
	dialogueOptionList* globalOptionList = initializeOptions(town);	
	int dialogueState = INITIAL_STATE;
	int npcBeingInterrogated = 0;
	int input = -1;
	//int currentQuestion = -1;
	//int currentNPC = -1; useful if commented out functions are included

	//TODO: This while loop is a place holder. It should be replaced with a "While the user doesn't press the quit dialogue button" loop. 
	while(npcBeingInterrogated != -1){
		if(CHOOSE_NPC_MODE){
			std::cout << "\n" << "Who to Interrogate?" << "\n";
			for(int i = 0; i < TOWN_SIZE; i++)
				std::cout << i << ") " << town[i].name << "\n";
			std::cin >> npcBeingInterrogated; //TODO: npcBeingInterrogated should be changed so that it is always set to the integer representing the position of the NPC being talked to in the town array. Perhaps this should be an added argument to the dialogue function. 
		}
		input = -1;
		globalOptionList[dialogueState].printState(); 
		//TODO: This while loop can be taken out after implementing a dialogue selection.
		while(input < 0 || input > 3){
			std::cin >> input; //TODO: This input represents a selection of a dialogue option, so replace it with an integer representation of each dialogue option. (e.g. a 0 represents the "who dislikes (murdered) question.)			
		}
		int tag = globalOptionList[dialogueState].options[input]->tag;
		dialogueState = globalOptionList[dialogueState].options[input]->nextTag;
		std::string output = "";
		if(tag == HOWFEEL){
			int npc = getNPC(town);
			output = howFeel(town, npc, npcBeingInterrogated);	
			std::cout << output; //TODO: Change std::cout to a blit to the screen.				
		}
		if(tag == WHENSEEN){
			int npc = getNPC(town);
			output = whenSeen(town,npc,npcBeingInterrogated);	
			std::cout << output; //TODO: Change std::cout to a blit to the screen.		
		}
		if(tag == HEARD){ 
			heard(town,npcBeingInterrogated);
		}
		if(tag == DONEWITH){
			bool isSelf = selfOrOthers(globalOptionList);
			int npc = getNPC(town);
			if(isSelf){
				doneWithSelf(town,npcBeingInterrogated, npc);
			}else{
				doneWith(town,npcBeingInterrogated,npc);
			}	
		}
			dialogueState = INITIAL_STATE; 
	}	
	return 0;
}
