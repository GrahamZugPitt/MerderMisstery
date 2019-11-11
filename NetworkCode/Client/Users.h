#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <fstream>
#include <streambuf>

using namespace std;

struct LinkedNode {
	string data;
	LinkedNode *next;
};

class Linkedlist {
	private:
		LinkedNode *head, *tail;
	public:
		Linkedlist() :head{nullptr}, tail{nullptr} {}
		void createNode(string d) {
			LinkedNode *temp = new LinkedNode;
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
			LinkedNode *temp = new LinkedNode;
			temp = head;
			int counter = 1;
			while(temp != nullptr) {
				cout << temp->data << endl;
				temp = temp->next;
				counter++;
			}
		}
		
};

class User{

	private:
		string username;
		string password;
		Linkedlist friend_list;

	public:

	User(string un, string pw, Linkedlist fl){
		username = un;
		password = pw;
		friend_list = fl;
	}

	User(){}

	void set_username(string un){
		username = un;
	}
	void set_password(string pw){
		password = pw;
	}
	void set_friends(string friend_string){
		string temp;
		stringstream stream(friend_string);
		while(getline(stream, temp, ',')){
			friend_list.createNode(temp);
		}
	}

	string get_username(){
		return username;
	}
	//delete this soon
	string get_password() {
		return password;
	}
	Linkedlist get_friends(){
		return friend_list;
	}
	bool check_password(string un, string pw) {
		if(username.compare(un) == 0 && password.compare(pw) == 0){
			return true;
		}
		return false;
	}
	User find_user(string un){
		//open the text file
		User user;
		ifstream file;
		file.open("users.txt");

		if(file.fail()) {
			cout << "Error: File did not exist, exiting" << endl;
			return user;
		}

		//for loop, line by line, reading usernames only
		string userline;
		string temp_user;
		while(getline(file, userline)){
			temp_user = userline.substr(0, userline.find(','));
			if(temp_user.compare(un) == 0){
				
				user.set_username(temp_user);
				userline.erase(0, temp_user.length() + 1);
				
				temp_user = userline.substr(0, userline.find(','));
				user.set_password(temp_user);
				userline.erase(0, temp_user.length() + 1);

				user.set_friends(userline);

				return user;
			}
		}
		return user;
	}

};

