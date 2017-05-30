/********************************************************************
 * svoip.c - Going to be a simple VOIP (Voice Over IP) application. *
 ********************************************************************
 * By Philip R. Simonson                                            *
 ********************************************************************
 */

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <mmsystem.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <conio.h>

#define ALIAS "random_str"
#define MYPORT "31337"

int record_file(void);
void *get_in_addr(struct sockaddr *sa);
int setup_server(int *sockfd, struct addrinfo *their_addr);
int setup_client(int *sockfd, const char *hostname, struct addrinfo *their_addr);
int send_file(int sockfd, struct sockaddr *their_addr, const char *filename);
int recv_file(int sockfd, struct sockaddr *their_addr, const char *filename);
int play_file(const char *filename);

int main(int argc, char *argv[])
{
	WSADATA wsaData;
	int sockfd;
	struct addrinfo addr;

	if(argc > 2) {
		fprintf(stderr, "Usage: %s [address]\n", argv[0]);
		return 0;
	}

	WSAStartup(0x0202, &wsaData);
	if(argc == 1) {
		if(setup_server(&sockfd, &addr))
			goto error;
		printf("IP   : %s\nPORT : %s\n", inet_ntoa(((struct sockaddr_in*)addr.ai_addr)->sin_addr),
			MYPORT);
		if(recv_file(sockfd, addr.ai_addr, "tmp.wav"))
			goto error;
		if(play_file("tmp.wav"))
			goto error;
		if(remove("tmp.wav"))
			goto error;
	} else {
		if(setup_client(&sockfd, argv[1], &addr))
			goto error;
		printf("IP   : %s\nPORT : %s\n", inet_ntoa(((struct sockaddr_in*)addr.ai_addr)->sin_addr),
			MYPORT);
		if(record_file())
			goto error;
		if(send_file(sockfd, addr.ai_addr, "tmp.wav"))
			goto error;
		if(remove("tmp.wav"))
			goto error;
	}
	if(sockfd != 0)
		close(sockfd);
	WSACleanup();
	return 0;

error:
	if(sockfd != 0)
		close(sockfd);
	WSACleanup();
	return 1;
}

void *get_in_addr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int setup_server(int *sockfd, struct addrinfo *their_addr)
{
	struct addrinfo hints, *servinfo, *p;
	int rv;
	
	printf("*****************************\n"
		"* Tiny VOIP Application     *\n"
		"* By 5n4k3                  *\n"
		"*****************************\n\n");

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_flags = AI_PASSIVE;

	if((rv = getaddrinfo(NULL, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all the results and bind to the first it can */
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if((*sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("listener: socket()");
			continue;
		}

		if(bind(*sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(*sockfd);
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
	*their_addr = *p;
	printf("listener: waiting to recvfrom...\n");

	return 0;
}

int setup_client(int *sockfd, const char *hostname, struct addrinfo *their_addr)
{
	struct addrinfo hints, *servinfo, *p;
	int rv;

	printf("*****************************\n"
		"* Tiny VOIP Application     *\n"
		"* By 5n4k3                  *\n"
		"*****************************\n\n");

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;

	if((rv = getaddrinfo(hostname, MYPORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	/* loop through all the results and make a socket */
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if((*sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("talker: socket()");
			continue;
		}

		break;
	}
	if(p == NULL) {
		fprintf(stderr, "talker: failed to create socket.\n");
		return 2;
	}
	freeaddrinfo(servinfo);
	*their_addr = *p;

	return 0;
}

int record_file(void)
{
	char mci_command[100];
	char retstr[300];
	int mci_error;

	/* open wav type alias */
	sprintf(mci_command, "open new type waveaudio alias %s", ALIAS);
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);

	/* set the time format */
	sprintf(mci_command, "set %s time format ms", ALIAS);
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);

	sprintf(mci_command, "record %s notify", ALIAS);
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);

	printf("Now on air, press a key to stop.\n");
	getch();

	sprintf(mci_command, "stop %s", ALIAS);
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);

	sprintf(mci_command, "save %s %s", ALIAS, "tmp.wav");
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);

	sprintf(mci_command, "close %s", ALIAS);
	mci_error = mciSendString(mci_command, retstr, sizeof(retstr), NULL);
	printf("Recording stopped.\nPress a key to quit.\n");
	getch();
	return mci_error;
}

#define CHUNK 1024

int send_file(int sockfd, struct sockaddr *their_addr, const char *filename)
{
	FILE *file;
	char buffer[CHUNK];
	char fname[BUFSIZ];
	char cwd[128];
	int total_bytes;
	int bytes;

	if(getcwd(cwd, sizeof(cwd)) == NULL) {
		fprintf(stderr, "Error: Cannot get current working directory.\n");
		return -2;
	}
	sprintf(fname, "%s\\%s", cwd, filename);
	if((file = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "Error: Cannot read input file.\n");
		return -1;
	}

	total_bytes = 0;
	while((bytes = fread(buffer, 1, CHUNK, file))) {
		if(bytes < 0) {
			perror("fread()");
			return -1;
		}
		if(sendto(sockfd, buffer, bytes, 0, their_addr, sizeof(*their_addr)) != bytes) {
			fprintf(stderr, "Couldn't send all of the data.\n");
			break;
		}
		total_bytes += bytes;
	}
	fclose(file);
	printf("Sent %d bytes of data.\n", total_bytes);

	return 0;
}

int recv_file(int sockfd, struct sockaddr *their_addr, const char *filename)
{
	FILE *file;
	char buffer[CHUNK];
	char fname[BUFSIZ];
	char cwd[128];
	int addrlen = sizeof(*their_addr);
	int total_bytes;
	int bytes;

	if(getcwd(cwd, sizeof(cwd)) == NULL) {
		fprintf(stderr, "Error: Cannot get current working directory.\n");
		return -2;
	}
	sprintf(fname, "%s\\%s", cwd, filename);
	if((file = fopen(fname, "wb")) == NULL) {
		fprintf(stderr, "Error: Cannot read input file.\n");
		return -1;
	}

	total_bytes = 0;
	while((bytes = recvfrom(sockfd, buffer, CHUNK, 0, their_addr, &addrlen))) {
		if(bytes < 0) {
			perror("recvfrom()");
			return -1;
		}
		if(fwrite(buffer, 1, bytes, file) != bytes) {
			fprintf(stderr, "Couldn't write all of the data.\n");
			break;
		}
		total_bytes += bytes;
	}
	fclose(file);
	printf("Received %d bytes of data.\n", total_bytes);

	return 0;
}

int play_file(const char *filename)
{
	if(PlaySound(filename, NULL, SND_FILENAME))
		return 1;
	return 0;
}
