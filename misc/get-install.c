/*
 *************************************************************
 * get-install.c - Program to get a specific url and download
 * that whatever the URL contains.
 * by 5n4k3
 *************************************************************
 */

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* function prototypes */
int dl_url(const char *host, char *path);
int get_line(char *s, int size);
char* strip_line(char *s, int size);

/* main() - entry point for all programs.
 */
int main(int argc, char **argv) {
	char buffer[BUFSIZ];
	char data[2][BUFSIZ];
	char *tmp[BUFSIZ];
	char *msgs[2] = {"hostname","url path"};
	int i;

	for(i = 0; i < 2; i++) {
		printf("Enter %s: ", msgs[i]);
		get_line(data[i], sizeof(data[i]));
	}
	for(i = 0; i < 2; i++) {
		tmp[i] = strip_line(data[i], BUFSIZ);
		printf("tmp %d: %s\n", i, tmp[i]);
	}
	return dl_url(tmp[0], tmp[1]); /* return success */
}

/* get_line() - gets a string from the user.
 */
int get_line(char *s, int size) {
	int c, i, j;

	j = i = 0;
	while((c = getchar()) != EOF && c != 0x0A)
		if(i < size-2) {
			if(c == 0x08) {	/* backspace found */
				*(s+i) = 0;
				--i;
				--j;
				continue;
			}
			*(s+i) = c;
			++i;
			++j;
		}
	if(c == 0x0A) {
		*(s+i) = c;
		++i;
		++j;
	}
	*(s+j) = 0;
	++j;
	return i;	/* return length of string */
}

/* strip_line() - strips off the line endings.
 */
char *strip_line(char *s, int size) {
	char *tmp = s;
	if((tmp = strchr(s, '\n')) != NULL)
		*tmp = 0x00;
	else {
		fprintf(stderr, "Error: Input to long for buffer.\n");
		return NULL;
	}
	return s;	/* return new char array */
}

/* defined HTTP header */
#define HTTP_HEADER "GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n"

/* dl_url() - downloads a file from a URL.
 */
int dl_url(const char *host, char *path) {
	FILE *file;
	struct addrinfo hints, *res;
	int sockfd;
	char data[BUFSIZ];
	char header[BUFSIZ];
	char fname[BUFSIZ];
	char *tmp, *savtok;
	int bytes;

	/* get host info, make socket and connect to it */
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	if(getaddrinfo(host, "80", &hints, &res) < 0) {
		fprintf(stderr, "Cannot get host address info.\n");
		return 1;
	}
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "Sorry couldn't create socket!\n");
		return 1;
	}
	printf("Socket created\nConnecting...\n");
	if(connect(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
		fprintf(stderr, "Could not connect to host.\n");
		close(sockfd);
		return 1;
	}
	puts("Connected!");
	memset(header, 0, sizeof(header));
	snprintf(header, sizeof(header), HTTP_HEADER, path, host);
	if(send(sockfd, header, strlen(header), 0) != strlen(header)) {
		fprintf(stderr, "Sending GET request failed.\n");
		close(sockfd);
		return 1;
	}
	puts("GET Sent...");
	memset(fname, 0, sizeof(fname));
	tmp = strtok(path, "/");
	while(tmp != NULL) {
		savtok = tmp;
		tmp = strtok(NULL, "/");
	}
	if(savtok != NULL)
		snprintf(fname, sizeof(fname), "%s", savtok);
	else
		snprintf(fname, sizeof(fname), "data.dat");
	if((file = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Cannot open output file for writing.\n");
		close(sockfd);
		return 1;
	}
	do {
		memset(data, 0, sizeof(data));
		bytes = recv(sockfd, data, sizeof(data), 0);
		if(bytes < 0) {
			fprintf(stderr, "Error: receiving data, trying again.\n");
			continue;
		} else {
			printf("Received %d bytes of data from %s.\n", bytes, host);
			if((bytes = fwrite(data, 1, bytes, file)) < 0)
				fprintf(stderr, "Error: writing to file %s.\n", fname);
			else
				printf("%d bytes written to file %s.\n", bytes, fname);
		}
	} while(bytes != 0);
	fclose(file);
	if(bytes == 0)
		puts("File transfer complete.");

	close(sockfd);
	return 0;	/* return success */
}
