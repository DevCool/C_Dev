/*
 **********************************************
 * Copyright (C) 2017 Philip R. Simonson.     *
 **********************************************
 */

#include <winsock2.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define REQSTR "GET /%s HTTP/1.0\r\nHost: http://%s/\r\nUser-Agent: fetch.c\r\n\r\n"

#define MAXREQ 512
#define MAXDATA 512

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	SOCKET sockfd;
	struct sockaddr_in addr;
	struct hostent hostinfo;
	char request[MAXREQ];
	char data[MAXDATA];
	int total_bytes;
	int bytes;

	if(argc < 2 || argc > 3) {
		fprintf(stderr, "Usage: %s [hostname] http_get", argv[0]);
		return 0;
	}

	WSAStartup(0x0202, &wsaData);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		fprintf(stderr, "Error couldn't create the socket.\n");
		return -1;
	}

	hostinfo = *gethostbyname("www.example.com");
	if(argc == 3)
		hostinfo = *gethostbyname(argv[1]);

	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(80);
	memcpy(&addr.sin_addr.s_addr, &hostinfo.h_addr, sizeof(hostinfo.h_length));

	if(connect(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		fprintf(stderr, "Error couldn't connect to socket.\n");
		goto error;
	}
	printf("Connected to : %s.\n", hostinfo.h_addr);

	memset(request, 0, sizeof(request));
	if(argc == 3)
		sprintf(request, REQSTR, argv[2], argv[1]);
	else
		sprintf(request, REQSTR, argv[1], "www.example.com");
	printf("%s", request);

	if(send(sockfd, request, strlen(request), 0) < 0) {
		fprintf(stderr, "Error: couldn't send the http get request to %s.\n",
			inet_ntoa(addr.sin_addr));
		goto error;
	} else {
		printf("Sent fetch message to server successfully!\n");
	}

	bytes = 0;
	total_bytes = 0;
	memset(data, 0, sizeof(data));
	while((bytes = recv(sockfd, data, sizeof(data), 0)) != 0) {
		if(bytes < 0) {
			fprintf(stderr, "Error: Recieving data failed.\n");
			goto error;
		}
		printf("%s", data);
		total_bytes += bytes;
		memset(data, 0, sizeof(data));
	}
	if(bytes == 0)
		puts("Connection closed by remote host.");
	printf("\n\nTotal received bytes is %d.\n", total_bytes);

	closesocket(sockfd);
	WSACleanup();
	return 0;

error:
	closesocket(sockfd);
	WSACleanup();
	return 1;
}
