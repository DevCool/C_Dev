#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WIN8

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define QUERYSTRING "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n"
#define HTTPADDR "http://www.nielshorn.net/_lib/download3.php?file=Hex-Ed.zip"
#define CHUNK 512

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	FILE *file;
	struct sockaddr_in remote;
	struct hostent *host;
	char data[CHUNK];
	char query[BUFSIZ];
	int total_bytes;
	int sockfd;
	int bytes;

	if(argc != 3) {
		fprintf(stderr, "Usage: %s <address> <page>\n", argv[0]);
		return 0;
	}
	if(WSAStartup(0x0202, &wsaData) != 0) {
		fprintf(stderr, "WSA Error code %ul\n", WSAGetLastError());
		return 1;
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("client: socket");
		WSACleanup();
		return 1;
	}

	/* get host by name */
	host = gethostbyname(argv[1]);

	/* clear memory (strings) */
	memset(data, 0, sizeof(data));
	memset(query, 0, sizeof(query));
	memset(&remote, 0, sizeof(remote));

	/* setup the sockaddr_in structure */
	remote.sin_family = AF_INET;
	remote.sin_port = htons(80);
	memcpy(&remote.sin_addr.s_addr, host->h_addr, sizeof(remote.sin_addr.s_addr));

	/* connect to socket */
	if(connect(sockfd, (struct sockaddr *)&remote, sizeof(remote)) < 0) {
		perror("client: connect");
		close(sockfd);
		WSACleanup();
		return -1;
	}

	/* open a file for writing */
	if((file = fopen("data.dat", "wb")) == NULL) {
		perror("fopen()");
		close(sockfd);
		WSACleanup();
		return -1;
	}

	/* query the server for a page */
	snprintf(query, sizeof(query), QUERYSTRING, argv[2], argv[1], "http_get.c");
	bytes = send(sockfd, query, strlen(query), 0);
	if(bytes < 0) {
		fprintf(stderr, "Error: couldn't send %d bytes.\n", bytes);
		fclose(file);
		remove("data.dat");
		close(sockfd);
		WSACleanup();
		return -1;
	}

	/* receive data from server */
	total_bytes = 0;
	while((bytes = recv(sockfd, data, CHUNK, 0)) != 0) {
		if(bytes < 0) {
			fprintf(stderr, "Warning couldn't receive %d bytes.", bytes);
			break;
		}
		printf("Received %d bytes, now writting...\n", bytes);
		bytes = fwrite(data, 1, CHUNK, file);
		if(bytes < 0) {
			fprintf(stderr, "Warning not all bytes were written.");
			break;
		}
		total_bytes += bytes;
	}
	if(bytes == 0)
		printf("Total bytes of data transfered: %d\nFile successfully wrote.\n",
			total_bytes);

	/* cleanup everything */
	fclose(file);
	close(sockfd);
	WSACleanup();
	return 0;
}
