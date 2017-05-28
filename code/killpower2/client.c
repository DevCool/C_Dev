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

extern void talker(int sockfd, struct addrinfo *server, char *message, size_t size);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	char request[BUFSIZ];
	char address[32];
	int rv;
	int bytes;

	if(argc < 1 || argc > 5) {
		fprintf(stderr, "Usage: %s [-t] address [message] [[username] [domain] [password]]\n", argv[0]);
		return -1;
	}

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	memset(address, 0, sizeof(address));
	WSAStartup(0x0202, &wsaData);
	if(argc == 4)
		strncpy(address, argv[2], sizeof(address));
	else
		strncpy(address, argv[1], sizeof(address));
	if ((rv = getaddrinfo(address, SERVERPORT, &hints, &servinfo)) != 0) {
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
	} else if(argc == 4) {
		if(argv[1][0] == '-') {
			switch(argv[1][1]) {
				case 't':
					printf("Talker...\n");
					talker(sockfd, servinfo, argv[3], strlen(argv[3]));
				break;
				default:
					printf("Unknown option '%c'.\n", argv[1][1]);
					printf("Options are: t\n");
			}
		} else {
			printf("Please use a '-' to add a switch.\n");
		}
		freeaddrinfo(servinfo);
	} else {
		printf("Running program on server side.\n");
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
