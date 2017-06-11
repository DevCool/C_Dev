#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WIN8
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

typedef unsigned long long int size_t;

void *get_in_addr(struct sockaddr *sa);
int get_file(int sockfd, struct sockaddr_storage *client, const char *buf, size_t size, int *total);

#define MYPORT "31337"
#define CHUNK 512

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	int sockfd;
	char buf[CHUNK];
	char s[128];
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr;
	socklen_t addrlen;
	int bytesRead;
	int bytesWritten;
	int total_bytes;
	int rv;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	WSAStartup(0x0202, &wsaData);
	if((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all the results and bind */
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("listener: socket()");
			continue;
		}
		if(bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("listener: bind()");
			continue;
		}
		break;
	}
	if(p == NULL) {
		fprintf(stderr, "listener: failed to bind socket\n");
		return 2;
	}
	freeaddrinfo(servinfo);

	printf("listener: waiting for data...\n");

	total_bytes = 0;
	addrlen = sizeof(their_addr);
	while(1) {
		if((bytesRead = recvfrom(sockfd, buf, CHUNK, 0,
			(struct sockaddr *)&their_addr, &addrlen)) != 0) {
			bytesWritten = get_file(sockfd, &their_addr, buf, bytesRead, &total_bytes);
		}
		if(bytesWritten == -1) {
			perror("fwrite()");
			close(sockfd);
			WSACleanup();
			exit(1);
		}
		if(bytesRead == 0)
			break;
	}

	printf("listener: got file from %s\n",
		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof(s)));
	printf("listener: packet is %d bytes in total.\n", total_bytes);

	close(sockfd);
	WSACleanup();
	return 0;
}

void *get_in_addr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int get_file(int sockfd, struct sockaddr_storage *client, const char *buf, size_t size, int *total)
{
	FILE *file;
	char filename[128];
	int bytes;

	if(getcwd(filename, sizeof(filename)) == NULL) {
		fprintf(stderr, "Couldn't get the current working directory.\n");
		close(sockfd);
		WSACleanup();
		exit(1);
	}
	strncat(filename, "\\temp.wav", sizeof(filename));
	if((file = fopen(filename, "wb")) == NULL) {
		fprintf(stderr, "Error: Cannot open file for writing.\n");
		close(sockfd);
		WSACleanup();
		exit(1);
	}
	if((bytes = fwrite(buf, 1, size, file)) > 0) {
		total += bytes;
	} else if(bytes < 0) {
		fprintf(stderr, "Error: Cannot write %d bytes to file.\n", bytes);
	}
	fclose(file);
	return bytes;
}
