#include <iostream>
#include <fstream>
#include <string>

using namespace std;

struct Post {
	string seed;
	string user;
	string text;
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
				cout << "Seed #" << counter << ": " << temp->data.seed << ", User: " <<temp->data.user << ", Message: " << temp->data.text << endl;
				temp = temp->next;
				counter++;
			}
		}
};


int main() {
	int c = -1;
	cout << "0 for read, 1 for write: ";
	cin >> c;
	
	if(c == 0) {	
		ifstream file("forum_data.txt");
		
		if(file.fail()) {
			ofstream create;
			create.open("forum_data.txt");
			cout << "File did not exist, file created" << endl;
			return 0;
		}
		
		if(!file) {
			cerr << "Error reading file" << endl;
			exit(1);
		}
	
		string line;
		int counter = 0;
		List d;
		Post p;
		while(getline(file, line)) {
			counter++;
			if(counter % 3 == 1) {
				p.seed = line;
			}
			else if(counter % 3 == 2) {
				p.user = line;
			}
			else {
				p.text = line;
				d.createNode(p);
			}
		}
		d.printList();
	}
	else {
		string seed, user, text;
		cout << "Enter seed: ";
		cin >> seed;
		cout << "\nEnter user: ";
		cin >> user;
		cout << "\nEnter text: ";
		if(cin.peek() == '\n') {
			cin.ignore();
		}
		getline(cin, text);
		
		ofstream file;
		file.open("forum_data.txt", ios_base::app);
		file << seed << "\n" << user << "\n" << text << "\n";
		
	}
	
	return 0;
}
