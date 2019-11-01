class EventNode {
private:
	Event* data;
public:
	EventNode(Event e);
	EventNode* prev;
	EventNode* next;
	Event getEvent();
	std::string toString();
	~EventNode();
}
EventNode:

class EventList {
private:
	EventNode* head;
	EventNode* tail;
	int size;
public:
	EventList(){
		size = 0;
		head = nullptr;
		tail = nullptr;
	}
	int getSize();
	void add();
	void insert();
	EventNode get();
	Event getEvent();
}

int main() {

}
