class EventNode {
private:
	Event* data;
public:
	EventNode(Event e);
	EventNode* prev;
	EventNode* next;
	Event getEvent();
	string toString();
	~EventNode();
}
EventNode:

class EventList {
private:
	EventNode* head;
	EventNode* tail;
	int size;
public:
	EventList();
	int getSize();
	void add();
	void insert();
	EventNode get();
	Event getEvent();
}

int main() {

}