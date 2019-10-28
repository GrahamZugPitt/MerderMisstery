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

    // Do-while loop to send and receive data
    char buf[4096];
    string userInput;
    FD_SET(sockfd, &master);
    bool firstTime = true;

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
                firstTime = false;
            }
            else
            {
            	break;
            }
	    }
    
    } while (userInput != "exit");

    //Shut her down
    close(sockfd);
    return 0;
}