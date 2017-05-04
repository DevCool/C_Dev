/*
 * This is purely for testing purposes. Not
 * an actual program.
 **********************************************
 */

/* Includes for socket programming */
#if defined(_WIN32)
	#include <windows.h>
	#include <winsock2.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
#endif

/* Standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

/* Defines for this program */
#define MAXBUF 1024
#define MY_PORT 4444

/* Custom error() function */
#define error(T) if((T)) { fprintf(stderr, "[ERROR] %s.\n", strerror(errno)); errno = 0; goto error; }

/* main() - entry point for this program.
 */
int main()
{
#if defined(_WIN32)
	WSADATA wsaData;
	ZeroMemory(&wsaData, sizeof(wsaData));
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

	struct sockaddr_in self;
	int serverfd, res;
	char buf[MAXBUF];

	/* --- Clear buffer and initialize socket --- */
	memset(buf, 0, sizeof(buf)/sizeof(char));
	serverfd=socket(AF_INET, SOCK_STREAM, 0);
	error(serverfd<0);

	/* --- Zero struct and set it up --- */
#if defined(_WIN32)
	ZeroMemory(&self, sizeof(self));
#else
	bzero(&self, sizeof(self));
#endif
	self.sin_family = AF_INET;
	self.sin_port = htons(MY_PORT);
	self.sin_addr.s_addr = INADDR_ANY;

    /* --- Bind port to socket --- */
    res=bind(serverfd, (struct sockaddr*)&self, sizeof(self));
    error(res<0);

    /* --- Listen for connections --- */
    res=listen(serverfd, 20);
    error(res<0);

    while(1) {
    	struct sockaddr_in client;
    	int clientlen=sizeof(client);
    	int clientfd;
    	char message[256];

    	clientfd=accept(serverfd, (struct sockaddr*)&client, &clientlen);
    	if(clientfd<0)
    		puts("Connection failed.");
		else if(clientfd>0) {
			memset(message, 0, sizeof(message)/sizeof(char));
			sprintf(message, "[%s:%u] connected.", inet_ntoa(client.sin_addr),
				self.sin_port);
			puts(message);
		}

#if defined(_WIN32)
		if(closesocket(clientfd)==0) {
#else
		if(close(clientfd)==0) {
#endif
			memset(message, 0, sizeof(message)/sizeof(char));
			sprintf(message, "[%s:%u] disconnected.", inet_ntoa(client.sin_addr),
				self.sin_port);
			puts(message);
		}
    }

#if defined(_WIN32)
	closesocket(serverfd);
	WSACleanup();
#else
	close(serverfd);
#endif
	return 0;

error:
#if defined(_WIN32)
	if(serverfd>0) closesocket(serverfd);
	WSACleanup();
#else
	close(serverfd);
#endif
    return 1;
}
