/******************************************************
 * A program to poweroff multiple computers across
 * a network.
 ******************************************************
 */

#if defined(__linux__)
	#include <sys/socket.h>
	#include <arpa/inet.h>
#else
	#include <windows.h>
	#include <winsock2.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define error(T) if((T)){ fprintf(stderr, "%s\n", strerror(errno)); errno = 0; goto error; }

DWORD (*funcThread)(LPVOID lpVoid);

DWORD prog_thread(LPVOID lpVoid)
{
#if defined(_WIN32)
    char *pname = "C:\\Windows\\system32\\shutdown.exe";
    const char *args[] = {"/s /t0", NULL};
#else
	char *pname = "/sbin/shutdown";
	const char *args[] = {"-P -h", NULL};
#endif
	execvp(pname, args);
	return 0;
}

int main()
{
	HANDLE hThread;
	struct sockaddr_in self;
	int sockfd;

#if defined(_WIN32)
	WSADATA wsaData;
	ZeroMemory(&wsaData, sizeof(wsaData));
	WSAStartup(MAKEWORD(2, 0), &wsaData);
#endif

	if((sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket()");
		WSACleanup();
		exit(1);
	}

	ZeroMemory(&self, sizeof(self));
	self.sin_family = AF_INET;
	self.sin_port = htons(8888);
	self.sin_addr.s_addr = INADDR_ANY;

    error(bind(sockfd, (struct sockaddr*)&self, sizeof(self)) < 0);
    error(listen(sockfd, 20) < 0);

    while(1) {
        struct sockaddr_in client;
        int clientlen = sizeof(client);
        int clientfd;
		clientfd=accept(sockfd, (struct sockaddr*)&client, &clientlen);
		if(clientfd > 0) {
#if defined(_WIN32)
			funcThread = &prog_thread;
			funcThread(NULL);
			closesocket(clientfd);
#else
			funcThread = &prog_thread;
			funcThread(NULL);
			close(clientfd);
#endif
		} else
			perror("accept()");
    }

#if defined(_WIN32)
	while(WaitForSingleObject(hThread, 0) == 0);
	closesocket(sockfd);
	WSACleanup();
#else
	close(sockfd);
#endif
    return 0;

error:
#if defined(_WIN32)
	closesocket(sockfd);
	WSACleanup();
#else
	close(sockfd);
#endif
	return 1;
}
