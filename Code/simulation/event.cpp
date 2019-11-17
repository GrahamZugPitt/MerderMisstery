/*
**
**	Event class
**	
*/ 

#include <iostream>
#include <fstream>
#include <map>
#include <time.h>
#include <stdlib.h>

#include "npc.cpp"


class Event{
	public:
		int time;
		std::string npcName1;
		std::string npcName2;
		Event(NPC npc1, NPC npc2, int t) {
			npcName1 = npc1.name;
			npcName2 = npc2.name
			time = t;
		}
		
		Event() {
			time = 0;
			npcName1 = "first";
			npcName2 = "second";
		}
		
		std::string toString() {
			std::string info = "";
			info += npcName1;
			info += "\n";
			info += npcName2;
			info += "\n";
			info += "Time: ";
			info += std::to_string(time);
			info += "\n"
			return info;
		}
		
		~Event() {}
};

int main(){
	Event a();
	std::cout << a.toString() << std::endl;
}