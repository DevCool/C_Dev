/********************************************************************
 * svoip.c - Going to be a simple VOIP (Speak Over NET) application *
 ********************************************************************
 * By Philip R. Simonson                                            *
 ********************************************************************
 */

#define WIN32_LEAN_AND_MEAN
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
#define MAXDATA 1024

typedef BOOL bool;

#define false FALSE
#define true TRUE

/* packet structs */
struct PACKET {
	char data[MAXDATA];
};
typedef struct PACKET packet_t;

struct FRAME {
	bool bArrived;
	packet_t packet;
};
typedef struct FRAME frame_t;

/* helper functions */
void clear_frame(frame_t *frame);
void *get_in_addr(struct sockaddr *sa);

/* setup functions */
int setup_server(void);
int setup_client(const char *hostname);

/* file functions */
int record_file(void);
int send_file(int sockfd, struct sockaddr_storage *client, const char *filename, frame_t *packet);
int recv_file(int sockfd, struct sockaddr_storage *server, const char *filename, frame_t *packet);
int play_file(const char *filename);

/* main() - entry point for application
 */
int main(int argc, char *argv[])
{
	WSADATA wsaData;
	int sockfd;
	struct sockaddr_storage client, addr;
	frame_t frame;

	if(argc > 2) {
		fprintf(stderr, "Usage: %s [address]\n", argv[0]);
		return 0;
	}

	memset(&client, 0, sizeof(client));
	memset(&addr, 0, sizeof(addr));
	clear_frame(&frame);

	WSAStartup(0x0202, &wsaData);
	if(argc == 1) {
		sockfd = setup_server();
		if(recv_file(sockfd, &client, "test.wav", &frame))
			goto error;
		printf("IP   : %s\nPORT : %s\n", inet_ntoa(((struct sockaddr_in *)&client)->sin_addr), MYPORT);
		if(play_file("tmp.wav"))
			goto error;
		if(remove("tmp.wav"))
			goto error;
	} else {
		sockfd = setup_client(argv[1]);
		puts("Press any key to record message...");
		getch();
		if(record_file() != 0)
			goto error;
		printf("IP   : %s\nPORT : %s\n", inet_ntoa(((struct sockaddr_in *)&addr)->sin_addr), MYPORT);
		if(send_file(sockfd, &addr, "tmp.wav", &frame) != 0)
			goto error;
		if(remove("tmp.wav") != 0)
			goto error;
	}
	if(sockfd != 0)
		closesocket(sockfd);
	WSACleanup();
	return 0;

error:
	if(sockfd != 0)
		closesocket(sockfd);
	WSACleanup();
	return 1;
}

void clear_frame(frame_t *frame)
{
	frame_t ftmp;
	memset(&ftmp, 0, sizeof(ftmp));
/*	memset(&ftmp.packet, 0, sizeof(ftmp.packet)); */
}

void *get_in_addr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET)
		return &(((struct sockaddr_in*)sa)->sin_addr);
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int setup_server(void)
{
	struct addrinfo hints, *servinfo, *p;
	int sockfd;
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
	printf("listener: waiting to recvfrom...\n");
	return sockfd;
}

int setup_client(const char *hostname)
{
	struct addrinfo hints, *servinfo, *p;
	int sockfd;
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
		if((sockfd = socket(p->ai_family, p->ai_socktype,
			p->ai_protocol)) == -1) {
			perror("talker: socket()");
			continue;
		}
		if(connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("client: connect()");
			continue;
		}
		break;
	}
	if(p == NULL) {
		fprintf(stderr, "talker: failed to create socket.\n");
		return 2;
	}
	freeaddrinfo(servinfo);
	puts("client: socket created.");
	return sockfd;
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

int send_file(int sockfd, struct sockaddr_storage *client, const char *filename, frame_t *frame)
{
	FILE *file;
	char *buf;
	char cwd[256];
	int client_len;
	int total_bytes;
	int bytes;

	if(getcwd(cwd, sizeof(cwd)) == NULL) {
		fprintf(stderr, "Error: Cannot get current working directory.\n");
		return -2;
	}
	strncat(cwd, "\\", sizeof(cwd));
	strncat(cwd, filename, sizeof(cwd));
	if((file = fopen(cwd, "rb")) == NULL) {
		fprintf(stderr, "Error: Cannot read input file.\n");
		close(sockfd);
		WSACleanup();
		exit(1);
	}

	bytes = 0;
	total_bytes = 0;
	frame->bArrived = false;
	while(frame->bArrived == 0) {
		bytes = fread(frame->packet.data, 1, MAXDATA, file);
		if(bytes > 0) {
			total_bytes += bytes;
			if(bytes == MAXDATA) {
				frame->bArrived = true;
			}
		}
		if(bytes == 0)
			break;
	}
	buf = malloc(sizeof(frame_t));
	if(!buf) {
		fprintf(stderr, "Out of memory.\n");
		fclose(file);
		close(sockfd);
		WSACleanup();
		exit(1);
	}
	memcpy(buf, frame, sizeof(frame_t));
	client_len = sizeof(struct sockaddr_storage);
	if(sendto(sockfd, buf, sizeof(frame_t), 0, (struct sockaddr *)client, client_len) == -1) {
		fprintf(stderr, "Couldn't send all of the data.\n");
	} else {
		printf("Sent %d bytes of data.\n", total_bytes);
	}
	free(buf);
	fclose(file);
	return 0;
}

int recv_file(int sockfd, struct sockaddr_storage *server, const char *filename, frame_t *frame)
{
	struct sockaddr_in addr;
	FILE *file;
	char cwd[256];
	int addrlen;
	int total_bytes;
	int bytes;

	if(getcwd(cwd, sizeof(cwd)) == NULL) {
		fprintf(stderr, "Error: Cannot get current working directory.\n");
		return -2;
	}
	strncat(cwd, "\\", sizeof(cwd));
	strncat(cwd, filename, sizeof(cwd));
	if((file = fopen(cwd, "wb")) == NULL) {
		fprintf(stderr, "Error: Cannot open file for writing.\n");
		return -1;
	}
	bytes = 0;
	total_bytes = 0;
	addr = *((struct sockaddr_in *)server);
	addrlen = sizeof(addr);
	while((bytes = recvfrom(sockfd, frame->packet.data, MAXDATA, 0, (struct sockaddr*)&addr,
		&addrlen)) != 0) {
		if(fwrite(frame->packet.data, 1, bytes, file) == -1) {
			fprintf(stderr, "Couldn't write all of the data.\n");
			break;
		}
		total_bytes += bytes;
	}
	fclose(file);
	if(bytes == 0) {
		printf("Received %d bytes of data.\n", total_bytes);
		frame->bArrived = 1;
	}

	return 0;
}

int play_file(const char *filename)
{
	if(PlaySound(filename, NULL, SND_FILENAME))
		return 1;
	return 0;
}
