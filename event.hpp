class Event{
	public:
		int time;
		std::string npcName1;
		std::string npcName2;
		Event(std::string n1, std::string n2, int t);
		
		Event();
		
		std::string toString();
		
		~Event();
};