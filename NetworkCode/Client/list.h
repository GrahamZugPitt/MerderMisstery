#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <string>
#include <sstream>

struct Post {
	std::string seed;
	std::string user;
	std::string text;
};

struct Node {
	Post data;
	Node *next;
};

class List {
	private:
		Node *head, *tail;
	public:
		List() :head{nullptr}, tail{nullptr} {}
		void createNode(Post d) {
			Node *temp = new Node;
			temp->data = d;
			temp->next = nullptr;
			if(head == nullptr) {
				head = temp;
				tail = temp;
				temp = nullptr;
			}
			else {
				tail->next = temp;
				tail = temp;
			}
		}
		void printList() {
			Node *temp = new Node;
			temp = head;
			int counter = 1;
			while(temp != nullptr) {
				std::cout << "Seed #" << counter << ": " << temp->data.seed << "\nUser: " << temp->data.user << "\nMessage: " 
				<< temp->data.text << "\n\n" << std::endl;
				temp = temp->next;
				counter++;
			}
		}
		std::string toString() {
			Node *temp = new Node;
			temp = head;
			std::stringstream ss;
			while(temp != nullptr) {
				ss << temp->data.seed << "\n" << temp->data.user << "\n" << temp->data.text << "\n";
				temp = temp->next;
			}
			return ss.str();
		}
};

#endif