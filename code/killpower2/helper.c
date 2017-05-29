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

typedef unsigned long long int size_t;

void talker(int sockfd, struct addrinfo *server, char *message)
{
	char buffer[BUFSIZ];
	struct sockaddr_in serv = *((struct sockaddr_in*)(server->ai_addr));
	int addrlen = sizeof(serv);
	int bytes;

	bytes = sendto(sockfd, message, strlen(message), 0,
			server->ai_addr, server->ai_addrlen);
	if(bytes < 0) {
		fprintf(stderr, "Couldn't send %d bytes to %s.\n",
			bytes, inet_ntoa(serv.sin_addr));
	} else {
		printf("%d bytes sent to %s.\n", bytes,
			inet_ntoa(serv.sin_addr));
	}

	memset(buffer, 0, sizeof(buffer));
	bytes = recvfrom(sockfd, buffer, BUFSIZ-1, 0,
			(struct sockaddr*)&serv, &addrlen);
	if(bytes < 0) {
		fprintf(stderr, "Lost %d bytes from %s.\n",
			bytes, inet_ntoa(serv.sin_addr));
	} else {
		printf("%d bytes received from %s.\nClient: %s\n",
			bytes, inet_ntoa(serv.sin_addr), buffer);
	}
}

void server_talker(int sockfd, struct sockaddr_in *client)
{
	int addrlen = sizeof(*client);
	char message[BUFSIZ];
	int bytes;

	memset(message, 0, sizeof(message));
	bytes = recvfrom(sockfd, message, BUFSIZ-1, 0,
			(struct sockaddr*)client, &addrlen);
	if(bytes < 0) {
		fprintf(stderr, "Lost %d bytes from %s.\n",
			bytes, inet_ntoa(client->sin_addr));
	} else {
		printf("%d bytes received from %s.\nClient: %s\n",
			bytes, inet_ntoa(client->sin_addr), message);
	}

	bytes = sendto(sockfd, message, strlen(message), 0,
			(struct sockaddr*)client, addrlen);
	if(bytes < 0) {
		fprintf(stderr, "Couldn't send %d bytes to %s.\n",
			bytes, inet_ntoa(client->sin_addr));
	} else {
		printf("%d bytes sent to %s.\n", bytes,
			inet_ntoa(client->sin_addr));
	}
}
