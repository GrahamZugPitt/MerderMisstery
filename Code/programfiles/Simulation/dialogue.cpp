#ifndef __DIALOGUE__
#define __DIALOGUE__

#include "dialogue.hpp"

const int DIALOGUE_OPTION_COUNT = 40;
const int NUMBER_OF_STATES = 10;
const int DIALOGUE_OPTIONS_PER_STATE = 4;
const int CHOOSE_NPC_MODE = 1;

// All the different "dialogue boxes" with relevant context info
std::string normaldiscussionBoxPath = "Art/DiscussionImages/DiaScrQ2.png";
std::string answerdiscussionBoxPath = "Art/DiscussionImages/DiaScrA2.png";
std::string multipleanswersdiscussionBoxPath = "Art/DiscussionImages/DiaScrAs2.png";
std::string chooseMurdererPath = "Art/DiscussionImages/Ghost2.png";

// Various images to be sprinkled
std::string selectedBoxPath = "Art/DiscussionImages/TextBoxSelected.png";
std::string deselectedBoxPath = "Art/DiscussionImages/TextBoxDeselected.png";
std::string singlePlayerPathDisc = "Art/Player/SinglePlayer.png";

SDL_Rect useless;

int nameboxw = 100;
int nameboxh = 50;

int boxw, boxh, width_offset, height_offset, tl_x, tl_y, tr_x, tr_y, bl_x, bl_y, br_x, br_y, w, h, selected_x, selected_y, multiple_answers_page;
int selected = 1;
NPClite *town;
bool triggerPressed_dialogue = false;

SDL_Texture* normaldiscussionBoxTex;
SDL_Texture* answerdiscussionBoxTex;
SDL_Texture* multipleanswersdiscussionBoxTex;
SDL_Texture* chooseMurdererTex;

SDL_Texture* selectedBoxTex;
SDL_Texture* deselectedBoxTex;
SDL_Texture* playerTex;
SDL_Texture* npcTex;
SDL_Texture* CharTex;

TTF_Font *TNR;
SDL_Surface *surfaceMessage_dialogue;
SDL_Texture *textureMessage_dialogue;

SDL_Rect NameBoxRect = {880 - (nameboxw / 2), 300, nameboxw, nameboxh};

SDL_Color White = {255, 255, 255};
SDL_Color Yellow = {255, 255, 0};

std::vector<std::string> multiple_answers_vec;
int answers_vec_size = 0;
int answer_vec_background_size = 0;

std::string TLString1;
std::string TRString1;
std::string TLString2;
std::string TRString2;

std::string TLString;
std::string TRString;
std::string BLString;
std::string BRString;
std::string ResponseString = "Greetings, Detective.";

const int text_size = 45;

int wonLost = 0;

// Render a string to a rectangle onscreen, wrapping around if the text is too long
void renderStringOnRect(std::string message, SDL_Rect insideRect, SDL_Renderer *renderer){
	// set message text surface and convert into texture, 1215 sets the text to
  // wrap to the next line at that pixel
  surfaceMessage_dialogue = TTF_RenderText_Blended_Wrapped(TNR, message.c_str(), Yellow, insideRect.w);
  textureMessage_dialogue = SDL_CreateTextureFromSurface(renderer, surfaceMessage_dialogue);

  int textW, textH;
  SDL_QueryTexture(textureMessage_dialogue, NULL, NULL, &textW, &textH);
  insideRect.w = textW;
  insideRect.h = textH;

  SDL_RenderCopy(renderer, textureMessage_dialogue, NULL, &insideRect);

  SDL_FreeSurface(surfaceMessage_dialogue);
  SDL_DestroyTexture(textureMessage_dialogue);
}

// Returns the index of the dead NPC
//		We need to talk about our naming conventions
int getNPC(){
	for(int i = 0; i < TOWN_SIZE; i++){
		if(town[i].isDead)
			return i;
	}
}

// This first huge chunk is pulled from the discussion file (which is no longer around)
// An initial function to get everything set up
void setup_vars(SDL_Texture *selected, SDL_Texture *discussionBoxTex){
  SDL_QueryTexture(selected, NULL, NULL, &boxw, &boxh);
  SDL_QueryTexture(discussionBoxTex, NULL, NULL, &w, &h);

  std::string vicName = town[getNPC()].name;
  TLString1 = "Who dislikes " + vicName + "?";
  TRString1 = "When was the last time you saw " + vicName + "?";
  BLString = "What gossip have you heard?";
  BRString = "What have you...?";
  TLString2 = "What have you seen " + vicName + " do with others?";
  TRString2 = "What have you done with " + vicName + "?";

  TLString = TLString1;
  TRString = TRString1;

  width_offset = (SCREEN_WIDTH - 2 * boxw) / 3;
  height_offset = (SCREEN_HEIGHT - h - 2 * boxh) / 3;

  tl_x = width_offset;
  tl_y = SCREEN_HEIGHT - h + height_offset;

  tr_x = tl_x + boxw + width_offset;
  tr_y = tl_y;

  bl_x = tl_x;
  bl_y = tl_y + boxh + height_offset - 25;

  br_x = tr_x;
  br_y = bl_y;
}

void draw_peoples(SDL_Renderer *renderer, SDL_Texture *playerTex, SDL_Texture *npcTex){
  renderTexture(renderer, playerTex, useless, 277, 36, 224, 288, false);

  SDL_Rect crop, pos;

  crop.x = 12;
	crop.y = 12;
	crop.w = 56;
	crop.h = 72;

  pos.x = 779;
	pos.y = 36;
	pos.w = 224;
	pos.h = 288;

  SDL_RenderCopy(renderer, npcTex, &crop, &pos);
}

void draw_boxes(int selectnum, SDL_Renderer *renderer, SDL_Texture *selected, SDL_Texture *deselected, int w, int h, bool bottom){
  if(selectnum != 1)
    renderTexture(renderer, deselected, useless, tl_x, tl_y, boxw, boxh, false);
  if(selectnum != 2)
    renderTexture(renderer, deselected, useless, tr_x, tr_y, boxw, boxh, false);
  if(selectnum != 3 && bottom)
    renderTexture(renderer, deselected, useless, bl_x, bl_y, boxw, boxh, false);
  if(selectnum != 4 && bottom)
    renderTexture(renderer, deselected, useless, br_x, br_y, boxw, boxh, false);

  switch(selectnum){
    case 1:
      selected_x = tl_x;
      selected_y = tl_y;
      break;
    case 2:
      selected_x = tr_x;
      selected_y = tr_y;
      break;
    case 3:
      selected_x = bl_x;
      selected_y = bl_y;
      break;
    default: // Only other possible case is 4 so this is an else
      selected_x = br_x;
      selected_y = br_y;
      break;
  }

  renderTexture(renderer, selected, useless, selected_x, selected_y, boxw, boxh, false);
}

// Just so I can use it in the below function
int getNPC();

void draw_choose_murderer_options(SDL_Renderer *renderer){
  int x = 50;
  int y = SCREEN_HEIGHT - h + 30;
  int dead = getNPC();

  int i;
  for(i = 0; i < 12; i++){
    SDL_Rect personBox = {x + (i % 4) * 296, y + (i / 4) * 95, 285, 80};
    if(selected - 1 == i){
      renderTexture(renderer, selectedBoxTex, useless, personBox.x, personBox.y, personBox.w, personBox.h, false);
    }
    else{
      if(i != dead)
        renderTexture(renderer, deselectedBoxTex, useless, personBox.x, personBox.y, personBox.w, personBox.h, false);
    }

    personBox.x += 50;
    personBox.y += 20;
    if(i != dead)
      renderStringOnRect(town[i].name, personBox, renderer);
  }
}

// The function to actually draw the text boxes on the screen
void draw_text(SDL_Renderer *renderer, SDL_Texture *TLBox, SDL_Rect TLBoxRect, SDL_Texture *TRBox, SDL_Rect TRBoxRect, SDL_Texture *BLBox, SDL_Rect BLBoxRect, SDL_Texture *BRBox, SDL_Rect BRBoxRect){
  SDL_RenderCopy(renderer, TLBox, NULL, &TLBoxRect);
  SDL_RenderCopy(renderer, TRBox, NULL, &TRBoxRect);
  SDL_RenderCopy(renderer, BLBox, NULL, &BLBoxRect);
  SDL_RenderCopy(renderer, BRBox, NULL, &BRBoxRect);
}

// End of Griffin's code, moving on to Graham's
// Note all the comments are Griffin's, because Graham doesn't comment his code *angery emoji*
// jk Graham didn't expect me to have to look at it so it's not his fault

// Setting up enums for use in the simulation & dialogue
enum dishonesty{
	LITERALLY_CANNOT_LIE = 5,
	SOMEWHAT_DISHONEST = 10,
	VERY_DISHONEST = 15,
};

// Referring to the four possible states of dialogue
//  in a normal conversation (not with the ghost)
enum stateOptions{
	INITIAL_FOUR,
  WHAT_HAVE_YOU_TWO,
  ONE_ANSWER,
  MULTIPLE_ANSWERS,
  GHOST
} dialogueState;

// Keeps track of which dialogue element entered one of the above states
//  Init four doesn't given an answer, so we don't need one for that
enum whoSentYou{
  INIT_ONE,
  INIT_TWO,
  INIT_THREE,
  WHAT_ONE,
  WHAT_TWO
} whereFrom;

enum USELESSENUM{
  NPCS1,
  NPCS2,
  NPCS3,
  NPCS4,
  ACTION1,
  ACTION2,
  ACTION3,
  ACTION4,
  SELF_OR_OTHERS,
  INITIAL_STATE
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

// Class to represent a dialogue option
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

// Container for dialogue options
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
		TLString = options[0]->option;
		TLString = options[1]->option;
		TLString = options[2]->option;
		TLString = options[3]->option;
	}
};

// Just sets up the default stuff for Graham's stuff. Not sure this is used anymore.
dialogueOptionList* initializeOptions(){
	std::string dead = town[getNPC()].name;
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

// Maps from the event enum to string reps
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

// Pretty much the same as before, just with talking to the player
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

// Returns a time rep of game time
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

void growVector(){
      if(answers_vec_size == answer_vec_background_size){
        answer_vec_background_size++;
        multiple_answers_vec.push_back("");
      }
}

void setVector(){
	multiple_answers_page = 0;
        answers_vec_size = 0;
	answer_vec_background_size = 0;
}

// Not 100% on why this one exists, but I'm sure as shit not touching it
bool compareNames(int npc1, std::string npc2){
	if(!town[npc1].name.compare(npc2))
		return true;
	return false;
}

int get_npc_number_from_name(std::string name){
  for(int i = 0; i < 12; i++){
    std::string town_name = town[i].name;
    if(town_name.compare(name) == 0) return i;
  }
}

// Print out an arbitrary event dialogue
std::string printEventDialogue(Event* e){
			return e->npcName1 + " " + eventConverterDialogue(e->event) + " " + e->npcName2 + " " + getTime(e->time) + " at " + locationConverter(e->location) + ". \n";
}

//
std::string printEventDialogueFPF(Event* e){
			return "I " + eventConverterDialogue(e->event) + " " + e->npcName2 + " " + getTime(e->time) + " at " + locationConverter(e->location) + ".";
}
std::string printEventDialogueFPS(Event* e){
			return e->npcName1 + " " + eventConverterDialogue(e->event) + " me " + getTime(e->time) + " at " + locationConverter(e->location) + ".";
}

std::string printEventDialogueGossip(Event* e){
			return "I heard " + e->npcName1 + " " + eventConverterDialogue(e->event) + " " + e->npcName2 + " " + getTime(e->time) + " at " + locationConverter(e->location) + ". \n";
}

bool suspeciousEvent(int event){
	if(event == BREAKUP || event == ROB || event == PHYSICALFIGHT) return true;
	return false;
}

bool slightlySuspeciousEvent(int event){
	if(event == ASKFORDISTANCE || event == VERBALFIGHT || event == ENVY) return true;
	return false;
}

bool sayPersonalEventMurderer(int event, int beingInterrogated){

	if(town[beingInterrogated].didMurder)

	if((!suspeciousEvent(event)
	|| town[beingInterrogated].personality[DISHONESTY] < SOMEWHAT_DISHONEST)
	&& (!slightlySuspeciousEvent(event)
	|| town[beingInterrogated].personality[DISHONESTY] < VERY_DISHONEST)){
		return true;
	}
	return false;
}

bool sayPersonalEvent(int event, int beingInterrogated){
	if(town[beingInterrogated].didMurder)
		return sayPersonalEventMurderer(event,beingInterrogated);
	if((!suspeciousEvent(event)
	|| town[beingInterrogated].personality[DISHONESTY] < LITERALLY_CANNOT_LIE)
	&& (!slightlySuspeciousEvent(event)
	|| town[beingInterrogated].personality[DISHONESTY] < SOMEWHAT_DISHONEST)){
		return true;
	}
	return false;
}

void doneWithSelf(int beingInterrogated, int npc){
	bool anyEvent = false;
	for(int i = 0; i < town[beingInterrogated].memories.getSize(); i++){
		Event* e = town[beingInterrogated].memories.getMemory(i);
		if(compareNames(npc,e->npcName1) && e->event != INTRODUCE && e->event != SOCIALIZE && !suspeciousEvent(e->event)){
			growVector();
			multiple_answers_vec.at(answers_vec_size++) = printEventDialogueFPS(e);
			anyEvent = true;
			}
		if(compareNames(npc,e->npcName2)
		&& e->event != INTRODUCE
		&& e->event != SOCIALIZE
		&& sayPersonalEvent(e->event, beingInterrogated)){
			growVector();
			multiple_answers_vec.at(answers_vec_size++) = printEventDialogueFPF(e);
			anyEvent = true;
			}
	}
	if(!anyEvent)
		ResponseString =  "I've only made small talk with " + town[npc].name + ".";
}

void doneWith(int beingInterrogated, int npc){
	bool anyEvent = false;
	for(int i = 0; i < town[beingInterrogated].observations.getSize(); i++){
		Event* e = town[beingInterrogated].observations.getMemory(i);
		if(compareNames(npc,e->npcName1) && e->event != INTRODUCE && e->event != SOCIALIZE){
			growVector();
			multiple_answers_vec.at(answers_vec_size++) = printEventDialogue(e);
			anyEvent = true;
			}
		if(compareNames(npc,e->npcName2) && e->event != INTRODUCE && e->event != SOCIALIZE){
			growVector();
			multiple_answers_vec.at(answers_vec_size++) = printEventDialogue(e);
			anyEvent = true;
			}
	}
	if(!anyEvent)
	ResponseString = "I've only ever seen " + town[npc].name + " make small talk with others.";
}

std::string howFeel(int npc, int beingInterrogated){
	std::string dislike = "";
	bool nobody = true;
	for(int i = 0; i < TOWN_SIZE; i++){
		if(hasDoneEventMutual(town, INTRODUCE,beingInterrogated,i) && town[i].relationships[npc] < 0){
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

// Sets up our string vector
void heard(int beingInterrogated){
  Event* gossip[town[beingInterrogated].hearSay.getSize()];
	int j = 0;
	for(int i = 0; i < town[beingInterrogated].hearSay.getSize(); i++){
		bool alreadyMentioned = false;
		for(int k = 0; k < j; k++){
			if(eventsAreEqual(town[beingInterrogated].hearSay.getMemory(i), gossip[k]))
			alreadyMentioned = true;
		}
		if(!alreadyMentioned && !wasInvolvedInEvent(town, town[beingInterrogated].hearSay.getMemory(i),beingInterrogated)){
      // If we found a gossip that we want to say, stick it into our vector
	growVector();
			multiple_answers_vec.at(answers_vec_size++) = printEventDialogueGossip(town[beingInterrogated].hearSay.getMemory(i));
      gossip[j++] = town[beingInterrogated].hearSay.getMemory(i);
		}
	}
}

std::string whenSeen(int npc, int beingInterrogated){
	ResponseString = "I can't remember when I last saw " + town[npc].name;
	int event = -1;
	int eventTime = -1;
	Event* e = new Event();
	bool wasObservation = false;
	for(int i = town[beingInterrogated].memories.getSize() - 1; i > -1; i--){
		if (wasInvolvedInEvent(town, town[beingInterrogated].memories.getMemory(i),npc) &&
				sayPersonalEvent(town[beingInterrogated].memories.getMemory(i)->event,beingInterrogated)){
			event = town[beingInterrogated].memories.getMemory(i)->event;
			eventTime = town[beingInterrogated].memories.getMemory(i)->time;
			e = town[beingInterrogated].memories.getMemory(i);
			break;
		}
	}
	if(town[beingInterrogated].observations.getSize()){
		for(int i = town[beingInterrogated].observations.getSize() - 1; i > -1; i--){
			if(wasInvolvedInEvent(town, town[beingInterrogated].observations.getMemory(i),npc) && town[beingInterrogated].observations.getMemory(i)->time > eventTime){
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
		ResponseString = "I " + eventConverterDialogue(event) + " " + e->npcName2 + " at " + locationConverter(e->location) + ". That was " + getTime(eventTime);
	return ResponseString;
	}
		if(!e->npcName2.compare(town[beingInterrogated].name)){
		ResponseString = e->npcName1 + " " + eventConverterDialogue(event) + " me" + " at " + locationConverter(e->location) + ". That was " + getTime(eventTime);
	return ResponseString;
	}
		ResponseString = e->npcName1 + " " + eventConverterDialogue(event) + " " + e->npcName2 + " at " + locationConverter(e->location) + ". That was " + getTime(eventTime);
	}
	return ResponseString;
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

// Handle input for the main conversation menu
void init_four_keyhandler(const Uint8 *keyState, int talkingToNum){
  keyState = SDL_GetKeyboardState(NULL);

  if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]){
    if(selected == 3) selected = 1;
    if(selected == 4) selected = 2;
  }

  if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]){
    if(selected == 2) selected = 1;
    if(selected == 4) selected = 3;
  }

  if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN]){
    if(selected == 1) selected = 3;
    if(selected == 2) selected = 4;
  }

  if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]){
    if(selected == 1) selected = 2;
    if(selected == 3) selected = 4;
  }

  // Pick one of the boxes
  if ((keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_RETURN])){
    // Need to only register space / enter once per press
    if(!triggerPressed_dialogue){
      triggerPressed_dialogue = true;
      // The first two options just dumps you into an answer
      if(selected == 1){
        int npc = getNPC(); // Gives you the dead NPC
        ResponseString = howFeel(npc, talkingToNum);
        dialogueState = ONE_ANSWER;
      }
      else if(selected == 2){
        int npc = getNPC();
        ResponseString = whenSeen(npc,talkingToNum);
        dialogueState = ONE_ANSWER;
      }
      else if(selected == 3){ // 3 will give you the first page of a (possibly) multi-page answer
        dialogueState = MULTIPLE_ANSWERS;
        setVector();

        heard(talkingToNum); // The function that creates our multi-answer vec - tbh it's a black box to me

        if(answers_vec_size == 0){
          ResponseString = "I haven't heard any gossip about " + town[getNPC()].name + ".";
          dialogueState = ONE_ANSWER;
        }
        else{
          ResponseString = multiple_answers_vec[0];
        }
      }
      else if(selected == 4){
        dialogueState = WHAT_HAVE_YOU_TWO;
        selected = 1;
        TLString = TLString2;
        TRString = TRString2;
      }
    }
  }
  else{
    triggerPressed_dialogue = false;
  }
}

// Handle input from the "What have you..." menu
void what_two_keyhandler(const Uint8 *keyState, int talkingToNum){
  keyState = SDL_GetKeyboardState(NULL);

  if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]){
    if(selected == 2) selected = 1;
  }

  if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]){
    if(selected == 1) selected = 2;
  }

  if (keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_RETURN]){
    if(!triggerPressed_dialogue){
      triggerPressed_dialogue = true;
      // Both are multi-page answers
      if(selected == 1){
        dialogueState = MULTIPLE_ANSWERS;
        setVector();

        doneWith(talkingToNum, getNPC());

        if(answers_vec_size == 0){
          ResponseString = "I've only seen " + town[getNPC()].name + " make smalltalk with people.";
          dialogueState = ONE_ANSWER;
        }
        else{
          ResponseString = multiple_answers_vec[0];
        }
      }
      else if(selected == 2){
        dialogueState = MULTIPLE_ANSWERS;
        setVector();

        doneWithSelf(talkingToNum, getNPC()); // The function that creates our multi-answer vec - tbh it's a black box to me

        if(answers_vec_size == 0){
          ResponseString = "I've only ever made smalltalk with " + town[getNPC()].name + ".";
          dialogueState = ONE_ANSWER;
        }
        else{
          ResponseString = multiple_answers_vec[0];
        }
      }
    }
  }
  else{
    triggerPressed_dialogue = false;
  }
}

// Handle input from the multiple answer dialogue menu
void multiple_answers_keyhandler(const Uint8 *keyState){
  keyState = SDL_GetKeyboardState(NULL);

  if(!triggerPressed_dialogue){
    // Flip through the multiple pages with left and right arrow keys
    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]){
      triggerPressed_dialogue = true;
      if(multiple_answers_page > 0){
         multiple_answers_page--;
         ResponseString = multiple_answers_vec[multiple_answers_page];
      }
    }

    if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]){
      triggerPressed_dialogue = true;
      if(multiple_answers_page < answers_vec_size - 1){
        multiple_answers_page++;
        ResponseString = multiple_answers_vec[multiple_answers_page];
      }
    }

    // Reset to the main page
    if (keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_RETURN]){
      triggerPressed_dialogue = true;
      dialogueState = INITIAL_FOUR;
      TLString = TLString1;
      TRString = TRString1;
    }
  }
  if(!(keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_RETURN]))
    triggerPressed_dialogue = false;
}

// Handle input when you're trying to choose the murderer
void ghost_keyhandler(const Uint8 *keyState){
  keyState = SDL_GetKeyboardState(NULL);
  int dead = getNPC() + 1;

  if(!triggerPressed_dialogue){
    if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP]){
      triggerPressed_dialogue = true;
      if(selected > 4 && selected - 4 != dead) selected -= 4;
    }

    if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT]){
      triggerPressed_dialogue = true;
      if((selected - 1) % 4 > 0 && selected - 1 != dead) selected -= 1;
    }

    if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN]){
      triggerPressed_dialogue = true;
      if(selected < 9 && selected + 4 != dead) selected += 4;
    }

    if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT]){
      triggerPressed_dialogue = true;
      if((selected - 1) % 4 < 3 && selected + 1 != dead) selected += 1;
    }

    // Pick one of the boxes
    if ((keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_RETURN])){
      triggerPressed_dialogue = true;
      // You win!!
      if(town[selected-1].didMurder){
        wonLost = 1;
      }
      else{
        wonLost = -1;
      }
    }
  }
}

// Handles key input for the usual NPC conversation
int key_handler(const Uint8 *keyState, int talkingToNum){
  // If you wanna quit, we're out, no problem
  keyState = SDL_GetKeyboardState(NULL);
  if (keyState[SDL_SCANCODE_Q]){
    SDL_DestroyTexture(normaldiscussionBoxTex);
    SDL_DestroyTexture(answerdiscussionBoxTex);
    SDL_DestroyTexture(multipleanswersdiscussionBoxTex);
    SDL_DestroyTexture(chooseMurdererTex);
    SDL_DestroyTexture(selectedBoxTex);
    SDL_DestroyTexture(deselectedBoxTex);
    SDL_DestroyTexture(playerTex);
    SDL_DestroyTexture(CharTex);
    return -1;
  }

  if(dialogueState == INITIAL_FOUR){
    init_four_keyhandler(keyState, talkingToNum);
  }
  else if(dialogueState == WHAT_HAVE_YOU_TWO){
    what_two_keyhandler(keyState, talkingToNum);
  }
  else if(dialogueState == MULTIPLE_ANSWERS){
    multiple_answers_keyhandler(keyState);
  }
  else if(dialogueState == ONE_ANSWER){
    // If we're only at the one answer
    keyState = SDL_GetKeyboardState(NULL);
    if (keyState[SDL_SCANCODE_SPACE] || keyState[SDL_SCANCODE_RETURN]){
        if(!triggerPressed_dialogue){
          triggerPressed_dialogue = true;
          dialogueState = INITIAL_FOUR;
          TLString = TLString1;
          TRString = TRString1;
        }
    }
    else{
      triggerPressed_dialogue = false;
    }
  }
  else if(dialogueState == GHOST){
    ghost_keyhandler(keyState);
  }

  return 1;
}

// Draws the text in the four init rectangles
void draw_init_text(SDL_Renderer *renderer){
  SDL_Rect target = {tl_x + 10, tl_y + 10, boxw - 20, boxh - 20};
  renderStringOnRect(TLString, target, renderer);
  target.x = tr_x + 10;
  target.y = tr_y + 10;
  renderStringOnRect(TRString, target, renderer);
  target.x = bl_x + 10;
  target.y = bl_y + 10;
  renderStringOnRect(BLString, target, renderer);
  target.x = br_x + 10;
  target.y = br_y + 10;
  renderStringOnRect(BRString, target, renderer);
}

// Draws the text in the two other rectangles
void draw_secondary_text(SDL_Renderer *renderer){
  SDL_Rect target = {tl_x + 10, tl_y + 10, boxw - 20, boxh - 20};
  renderStringOnRect(TLString, target, renderer);
  target.x = tr_x + 10;
  target.y = tr_y + 10;
  renderStringOnRect(TRString, target, renderer);
}

// Draws the text when responding to the player's question
void draw_response_text(SDL_Renderer *renderer){
  SDL_Rect target = {100, SCREEN_HEIGHT - h + 100, w - 200, h - 200};
  renderStringOnRect(ResponseString, target, renderer);
}

// Draws the model to the screen.
void draw_handler(SDL_Renderer *renderer){
  SDL_RenderClear(renderer);
  // Draw the menu box, detective, npc, and npc's name every time for sure
  draw_peoples(renderer, playerTex, npcTex);

  // Get the NPC's name on the screen (also for sure)
  SDL_RenderCopy(renderer, CharTex, NULL, &NameBoxRect);

  // Then the conditional stuff
  if(dialogueState == INITIAL_FOUR){
    renderTexture(renderer, normaldiscussionBoxTex, useless, 0, SCREEN_HEIGHT - h, w, h, false);
    draw_boxes(selected, renderer, selectedBoxTex, deselectedBoxTex, w, h, true);
    draw_init_text(renderer);
    //printf("Selected is: %d\n", selected);
    //printf("1: %s\n", TLString.c_str());
    //printf("2: %s\n", TRString.c_str());
    //printf("3: %s\n", BLString.c_str());
    //printf("4: %s\n", BRString.c_str());
  }
  else if(dialogueState == WHAT_HAVE_YOU_TWO){
    renderTexture(renderer, normaldiscussionBoxTex, useless, 0, SCREEN_HEIGHT - h, w, h, false);
    draw_boxes(selected, renderer, selectedBoxTex, deselectedBoxTex, w, h, false);
    draw_secondary_text(renderer);
    //printf("Selected is: %d\n", selected);
    //printf("1: %s\n", TLString.c_str());
    //printf("2: %s\n", TRString.c_str());
  }
  else if(dialogueState == ONE_ANSWER){
    renderTexture(renderer, answerdiscussionBoxTex, useless, 0, SCREEN_HEIGHT - h, w, h, false);
    draw_response_text(renderer);
    //printf("ResponseString: %s\n", ResponseString.c_str());
  }
  else if(dialogueState == MULTIPLE_ANSWERS){
    renderTexture(renderer, multipleanswersdiscussionBoxTex, useless, 0, SCREEN_HEIGHT - h, w, h, false);
    draw_response_text(renderer);
    //printf("On page %d of %d\n", multiple_answers_page + 1, answers_vec_size);
    //printf("ResponseString: %s\n", ResponseString.c_str());
  }
  else if(dialogueState == GHOST){
    renderTexture(renderer, chooseMurdererTex, useless, 0, SCREEN_HEIGHT - h, w, h, false);
    draw_choose_murderer_options(renderer);
  }

  //Update Screen
  SDL_RenderPresent(renderer);
}

// The function that gets called when it's time to get down to work for reals
void dialogue(NPClite* our_town, SDL_Event e, bool *quit, const Uint8 *keyState, SDL_Renderer* renderer, NPC *theNPC){
	// Discussion Code
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // No bluriness on resize
	// Load the textures necessary for the menu
  normaldiscussionBoxTex = loadFiles(normaldiscussionBoxPath, renderer);
  answerdiscussionBoxTex = loadFiles(answerdiscussionBoxPath, renderer);
  multipleanswersdiscussionBoxTex = loadFiles(multipleanswersdiscussionBoxPath, renderer);
  chooseMurdererTex = loadFiles(chooseMurdererPath, renderer);

  selectedBoxTex = loadFiles(selectedBoxPath, renderer);
  deselectedBoxTex = loadFiles(deselectedBoxPath, renderer);
  playerTex = loadFiles(singlePlayerPathDisc, renderer);
  npcTex = theNPC->texture;

  town = our_town;

	// Initialize a bunch of values
  setup_vars(selectedBoxTex, normaldiscussionBoxTex);

	// Set up the text variables, values, etc
  TNR = TTF_OpenFont("Art/Font/times-new-roman.ttf", text_size); // Opens the font and sets the size
  if(!TNR)
    printf("TTF_OpenFont error: %s\n", TTF_GetError());

	// Create the surfaces for the text messages
  SDL_Surface *CharNameSurface = TTF_RenderText_Solid(TNR, theNPC->getName().c_str(), Yellow);
  CharTex = SDL_CreateTextureFromSurface(renderer, CharNameSurface);
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

	if(theNPC->isGhost)
    dialogueState = GHOST;
  else
    dialogueState = INITIAL_FOUR;

  // Will just keep looping until you quit
	while(true){
    // You won!
    if(wonLost == 1){
      runWinScreen(e, quit, keyState, renderer);
    }// Or you lost!
    else if(wonLost == -1){
      runLoseScreen(e, quit, keyState, renderer);
    }

    // This way we're not doing a bunch of needless drawing
    if(SDL_PollEvent(&e) != 0){
      //Quit application
      if(e.type == SDL_QUIT){
          *quit = true;
          return;
      }

      // INPUT HANDLER
      int shouldIquit = key_handler(keyState, get_npc_number_from_name(theNPC->getName()));
      if(shouldIquit == -1) return; // -1 means yes, I should quit

      // DRAW HANDLER
      draw_handler(renderer);
    }
    else if(triggerPressed_dialogue){
      triggerPressed_dialogue = false;
    }
	}
}

#endif
