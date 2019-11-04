#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <sstream>
#include <unistd.h>
#include "list.h"

#include <arpa/inet.h>

#define PORT "9034" 
#define STDIN 0

using namespace std;

int main(int argc, char *argv[])
{
    int sockfd, fdmax;
    struct addrinfo hints, *servinfo, *p;
    struct timeval tv;
    int rv;
    char s[INET6_ADDRSTRLEN];
    fd_set master;
    fd_set temp;

    tv.tv_sec = 0;

    FD_ZERO(&master);
    FD_ZERO(&temp);
    FD_SET(STDIN, &master);

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    freeaddrinfo(servinfo);
	int choice = -1;
	cout << "Enter 0 for chat and 1 for game seed sharing: ";
	cin >> choice;
	
	if(choice == 0) {
	
		// Do-while loop to send and receive data
		char buf[4096];
		string userInput;
		FD_SET(sockfd, &master);

		do
		{
			temp = master;

			select(sockfd+1, &temp, nullptr, nullptr, &tv);

			if (FD_ISSET(STDIN, &temp))
			{
				getline(cin, userInput);

				if (userInput.size() > 0) // Make sure the user has typed in something
				{
					// Send the text
					int sendResult = send(sockfd, userInput.c_str(), userInput.size() + 1, 0);
					if (sendResult <= 0)
					{
						break;
					}
				}
			}
			else
			{
				// Get response
				memset(&buf, 0, sizeof buf);
				int bytesReceived = recv(sockfd, buf, 4096, 0);
				if (bytesReceived > 0)
				{
					cout << string(buf, 0, bytesReceived) << endl;
				}
				else
				{
					break;
				}
			}
		
		} while (userInput != "exit");
	}
	else {
		char buf[4096];
		cout << "\n";
		
		//Receive data from server (to get past initial message)
		memset(&buf, 0, sizeof buf);
		int bytesReceived = recv(sockfd, buf, 4096, 0);
		
		//Randomly generated string, if server sees this then knows to send specific data
		string input = "uDN0oyTNjaAENW9KLocdjXP85ou3bAtBAhBAOPXLU8P0Iip8AcaELgAvewFNTpzM";
		int sendResult = send(sockfd, input.c_str(), input.size() + 1, 0);
		
		List posts;
		Post p;
		
		//Receive data from server
		memset(&buf, 0, sizeof buf);
		bytesReceived = recv(sockfd, buf, 4096, 0);
		
		//Put data into linked list and print
		if(bytesReceived > 1) {
			int i = 0;
			int counter = 0;
			string add = "";
			
			while(buf[i] != '\0') {
				if(buf[i] == '\n') {
					counter++;
					if(counter % 3 == 1) {
						p.seed = add;
						add = "";
					}
					else if(counter % 3 == 2) {
						p.user = add;
						add = "";
					}
					else {
						p.text = add;
						posts.createNode(p);
						add = "";
					}
				}
				else {
					add += buf[i];
				}
				i++;
			}
			posts.printList();
		}
		else {
			cout << "No seeds shared yet\n" << endl;
		}
		
		choice = 0;
		while(choice == 0) {
			cout << "Enter 0 to share a game seed or 1 to exit: ";
			cin >> choice;
			
			if(choice != 0) {
				break;
			}
			
			cout << "\nEnter Game Seed: ";
			cin >> p.seed;
			cout << "\nEnter User: ";
			cin >> p.user;
			cout << "\nEnter Message: ";
			if(cin.peek() == '\n') {
				cin.ignore();
			}
			getline(cin, p.text);
			posts.createNode(p);
			
			input = posts.toString();
			input += "uDN0oyTNjaAENW9KLocdjXP85ou3bAtBAhBAOPXLU8P0Iip8AcaELgAvewFNTpzM";
			
			sendResult = send(sockfd, input.c_str(), input.size() + 1, 0);
			
			memset(&buf, 0, sizeof buf);
			bytesReceived = recv(sockfd, buf, 4096, 0);
			
			if(bytesReceived > 0) {
				cout << "\n\n" << buf << endl;
				cout << "Updated List\n" << endl;
				posts.printList();
			}
		}
	}

    //Shut her down
    close(sockfd);
    return 0;
}
