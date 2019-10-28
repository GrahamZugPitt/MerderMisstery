#ifndef _EVENT_LIST_H
#define _EVENT_LIST_H

#include <stdio.h>
#include <iostream>

#include "event.hpp"

class EventNode {
private:
	Event* data;
public:
	EventNode(Event *e);
	EventNode* prev;
	EventNode* next;
	Event* getEvent();
	std::string toString();
	~EventNode();
};

class EventList {
private:
	EventNode* head;
	EventNode* tail;
	int size;
public:
	EventList();
	int getSize();
	void add(Event *e);
	void insert();
	EventNode get();
	Event getEvent();
	~EventList();
};


#endif