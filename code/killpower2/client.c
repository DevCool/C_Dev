/*****************************************************
 * KillPower 2 - Broadcasting Killpower
 *****************************************************
 * Copyright (C) 2017, Philip Simonson.
 *****************************************************
 */

#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SERVERPORT "8888"

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	char request[BUFSIZ];
	int rv;
	int bytes;

	if(argc != 2 && argc != 5) {
		fprintf(stderr, "Usage: %s address [[username] [domain] [password]]\n", argv[0]);
		return -1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	WSAStartup(0x0202, &wsaData);
	if ((rv = getaddrinfo(argv[1], SERVERPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all results and make a socket */
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("client: socket");
			continue;
		}
		break;
	}
	if(p == NULL) {
		fprintf(stderr, "client: failed to create socket\n");
		goto error;
	}

	if(argc == 5) {
		memset(request, 0, sizeof(request));
		snprintf(request, sizeof(request), "username=%s;domain=%s;password=%s;", argv[2], argv[3], argv[4]);
		if((bytes = sendto(sockfd, request, strlen(request), 0,
				p->ai_addr, p->ai_addrlen)) == -1) {
			perror("client: sendto");
			goto error;
		}
		freeaddrinfo(servinfo);

		printf("data sent: %s\n", request);
		printf("client: sent %d bytes to %s\n", bytes, argv[1]);
	}
	closesocket(sockfd);
	WSACleanup();
	return 0;

error:
	freeaddrinfo(servinfo);
	closesocket(sockfd);
	WSACleanup();
	return 1;
}
