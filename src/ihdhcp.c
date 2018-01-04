// ihdhcp : Ian's Humble DHCP server
//          right now just a very humble server that listens for input
//          when any input is received, it spits it to stdout and then closes

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "dbg.h"

int main (int argc, char *argv[])
{
	int sockfd, newsockfd, portno;
	socklen_t clientLength;

	char buffer[256];
	struct sockaddr_in serverAddress, clientAddress;	
	char *ptr;
	int n;

	check(argc == 2, "Usage: %s port", argv[0]);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	check(sockfd >= 0, "Error opening socket.");
	bzero((char *) &serverAddress, sizeof(serverAddress));
	portno = strtol(argv[1], &ptr, 10);
	
	debug("Port: %d", portno);

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;
	serverAddress.sin_port = htons(portno);
	
	check((bind(sockfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) >= 0),
		"Error binding socket");
	
	listen(sockfd, 5);
		
	clientLength = sizeof(clientAddress);
	newsockfd = accept(sockfd, (struct sockaddr *) &clientAddress, &clientLength);

	check(newsockfd >= 0, "Error accepting data");
	bzero(buffer, sizeof(buffer));
	n = read(newsockfd, buffer, sizeof(buffer) - 1);
	check(n >= 0, "Error reading from socket");
	printf("Message: %s\n", buffer);



	close(sockfd);
	close(newsockfd);

	return 0;

error:
	return 1;
	close(sockfd);
	close(newsockfd);
}
