#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

extern BOOL CreateRemoteProcess(const char *username, const char *domain,
		const char *password, DWORD logonType, DWORD logonProvider);

int main(int argc, char *argv[])
{
	char buffer[BUFSIZ];
	char user[128];
	char pass[128];
	char domain[128];

	WSADATA wsaData;
	SOCKET sockfd;
	struct sockaddr_in server, client;
	int bytes;

	WSAStartup(0x0202, &wsaData);
	if((sockfd = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, 0)) == INVALID_SOCKET) {
		fprintf(stderr, "Error: WSASocket() - couldn't create socket.\n");
		return 1;
	}

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(8888);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
		perror("listener: bind()");
		goto error;
	}
	puts("listener: waiting to recvfrom...");

	int clilen = sizeof(client);
	memset(buffer, 0, sizeof(buffer));
	memset(user, 0, sizeof(user));
	memset(domain, 0, sizeof(domain));
	memset(pass, 0, sizeof(pass));
	if((bytes = recvfrom(sockfd, buffer, BUFSIZ-1, 0, (struct sockaddr*)&client, &clilen)) < 0) {
		perror("recvfrom()");
		goto error;
	}
	if(bytes > 0)
		printf("received: %s\n", buffer);
	else if(bytes == -1)
		goto error;

	sscanf(buffer, "username=%s;domain=%s;password=%s;", user, domain, pass);
	if(!CreateRemoteProcess(user, domain, pass, LOGON32_LOGON_SERVICE,
			LOGON32_PROVIDER_WINNT50))
		goto error;

	closesocket(sockfd);
	WSACleanup();
	return 0;

error:
	closesocket(sockfd);
	WSACleanup();
	return 1;
}
