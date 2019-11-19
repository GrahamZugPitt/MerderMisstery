#include <iostream>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string>
#include <cstring>
#include <sstream>
#include <fstream>
#include <streambuf>
#include <unistd.h>

#include "Users.h"

using namespace std;

#define PORT "9034"

int main()
{   
    fd_set master;    // master file descriptor list
    fd_set copy;  // temp file descriptor list for select()
    int fdmax;        // maximum file descriptor number

    int listener;     // listening socket descriptor
    int client;        // newly accept()ed client socket descriptor
    int temp;         // temp socket descriptor
    struct sockaddr_storage remoteaddr; // client address
    socklen_t addrlen;

    char remoteIP[INET6_ADDRSTRLEN];

    int yes=1;        // for setsockopt() SO_REUSEADDR, below
    int i, j, rv;

    struct addrinfo hints, *ai, *p;

    ChatBank chat;

    FD_ZERO(&master);    // clear the master and temp sets
    FD_ZERO(&copy);

    // get us a socket and bind it
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(NULL, PORT, &hints, &ai)) != 0) {
        fprintf(stderr, "selectserver: %s\n", gai_strerror(rv));
        exit(1);
    }
    
    for(p = ai; p != NULL; p = p->ai_next) {
        listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener < 0) { 
            continue;
        }
        
        // lose "address already in use" error message
        setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

        if (bind(listener, p->ai_addr, p->ai_addrlen) < 0) {
            close(listener);
            continue;
        }

        break;
    }

    // if we got here, it means we didn't get bound
    if (p == NULL) {
        fprintf(stderr, "selectserver: failed to bind\n");
        exit(2);
    }

    freeaddrinfo(ai); 

    //Navi: Hey, listen!
    if (listen(listener, 10) == -1) {
        perror("listen");
        exit(3);
    }

    // add the listener to the master set
    FD_SET(listener, &master);

    //this the max so far
    fdmax = listener;

    // keep track of the biggest file descriptor
    fdmax = listener; // so far, it's this one
    bool running = true;

    while(running)
    {
        copy = master;

        // See who's talking to us
        int socketCount = select(fdmax+1, &copy, nullptr, nullptr, nullptr);

        // Loop through all the current connections / potential connections
        for (int i = 0; i <= fdmax; i++)
        {
        	if (FD_ISSET(i, &copy))
        	{
	            temp = i;

	            // Is it an inbound communication?
	            if (temp == listener)
	            {
	                // Accept a new connection
	                client = accept(listener, nullptr, nullptr);

	                // Add the new connection to the list of connected clients
	                FD_SET(client, &master);

	                //check to see if it's the max
	                if(client > fdmax){
	                	fdmax = client;
	                }

	                // Send a welcome message to the connected client
	                string welcomeMsg = "\nWelcome to Merder Misstery chat!\r\n";
	                send(client, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);
	            }
	            else // It's an inbound message
	            {
	                char buf[4096];
	                memset(&buf, 0, sizeof buf);
	                
	                // Receive message
	                int bytesIn = recv(temp, buf, 4096, 0);
	                if (bytesIn <= 0)
	                {
	                    // Drop the client
	                    close(temp);
	                    FD_CLR(temp, &master);
	                }
	                else
	                {
	                    //Client is looking for specific data
						string buffer(buf);
						if(buffer == "uDN0oyTNjaAENW9KLocdjXP85ou3bAtBAhBAOPXLU8P0Iip8AcaELgAvewFNTpzM") {
							ifstream file;
							file.open("forum_data.txt");
							
							if(file.fail()) {
								ofstream create;
								create.open("forum_data.txt");
								cout << "File did not exist, file created" << endl;
							}
						
							string data;
							file.seekg(0, ios::end);
							data.reserve(file.tellg());
							file.seekg(0, ios::beg);
							data.assign((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
							file.close();
							
							//Send back to listener
							for(int i = 0; i <= fdmax; i++) {
								int outSock = i;
								if(outSock == listener || outSock == temp) {
									ostringstream ss;
									ss << data;
									string strOut = ss.str();
									
									send(outSock, strOut.c_str(), strOut.size() + 1, 0);
								}
							}
						}
						//Client wants to send the updated data
						else if(buffer.size() > 64 && buffer.substr(buffer.size()-64).compare("uDN0oyTNjaAENW9KLocdjXP85ou3bAtBAhBAOPXLU8P0Iip8AcaELgAvewFNTpzM") == 0) {
							ofstream file;
							file.open("forum_data.txt", ios::out | ios::trunc);
							file << buffer.substr(0, buffer.size()-64);
							file.close();
							
							//Send back to listener
							for(int i = 0; i <= fdmax; i++) {
								int outSock = i;
								if(outSock == listener || outSock == temp) {
									string strOut = "Data updated successfully";
									
									send(outSock, strOut.c_str(), strOut.size() + 1, 0);
								}
							}
						}
						else {
							//change the chat data structure and send out to the chat UI
							chat.addChat(buf);

							//test line displaying to the server the current state of the data structure
							chat.printComments();
							printf("\n");
							string str = "hello";


							//might need to change the case of the length of the message, in client code?

							// Send message to all clients, and not listening socket
							for (int i = 0; i <= fdmax; i++)
							{
								int outSock = i;
								if (outSock != listener)
								{
									send(outSock, chat.returnComments().c_str(), chat.returnComments().size(), 0);
								}
							}
						}
	                }
	            }
	        }
        }
    }

    // Remove the listening socket from the master file descriptor set and close it
    // to prevent anyone else trying to connect.
    FD_CLR(listener, &master);
    close(listener);
    
    // Message to let users know what's happening.
    string msg = "Server is shutting down\r\n";

    for (int i = 0; i < fdmax; i++)
    {
        // Get the socket number
        int sock = i;

        // Send the goodbye message
        send(sock, msg.c_str(), msg.size() + 1, 0);

        // Remove it from the master file list and close the socket
        FD_CLR(sock, &master);
        close(sock);
    }
    return 0;
}
