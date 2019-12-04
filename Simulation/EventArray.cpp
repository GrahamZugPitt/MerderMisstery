class EventArray {
private:
	Event* memories;
	int size;
	int maxSize;
public:
	EventArray(){
		size = 0;
		this->memories = new Event[1000]; 
		maxSize = 99;
	}
	void wipe(){
		for(int i = 0; i < 1000; i++){
			memories[i] = *(new Event());
		}
	}
	void expand(){
		Event* temp = new Event[2*size];
		for(int i = 0; i < size; i++){
			temp[i] = memories[i];
		}
		memories = temp;
	}
	int getSize(){ //returns size
		return size;
	}
	Event* getMemory(int time){ //returns a specific memory
		return &this->memories[time];
	}
	void add(Event* e){ //adds an event to the memory set
		this->memories[size] = *e;
		size++;
		}
};

