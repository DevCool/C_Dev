/************************************
 * Simple Multi-Socket Server v0.01 *
 * by Philip Simonson               *
 ************************************
 */

#include <winsock2.h>
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int main(argc,argv)
	int argc;
	char **argv;
{
	WSADATA wsaData;
	int clientlen;
	int clientfd;
	int sock;
	struct sockaddr_in server,client;
	int i;

	if(argc < 2 || argc > 3) {
		printf("Usage: %s address [port]\n",argv[0]);
		return 1;
	}

	WSAStartup(0x0202,&wsaData);

	/* setup server and create socket */
	if((sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
		perror("socket()");
		return 1;
	}

	int port = 23;
	if(argc == 3)
		port = atoi(argv[2]);
	ZeroMemory(&server,sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(argv[1]);

	/* bind port to socket */
	if(bind(sock,(struct sockaddr*)&server,sizeof(server)) < 0) {
		perror("bind()");
		goto error;
	}

	/* listen for connections */
	if(listen(sock,1) < 0) {
		perror("listen()");
		goto error;
	}

	clientlen = sizeof(client);
	clientfd = accept(sock,(struct sockaddr*)&client,&clientlen);
	if(clientfd < 0) {
		perror("accept()");
		goto error;
	}
	while(clientfd > 0) {
		char c;
		puts("Client connected...");
		if(send(clientfd,"Hello world!\n\r",14,0) != 14) {
			printf("Cannot send 14 bytes of data to %s:%d.\n",
				inet_ntoa(client.sin_addr),ntohs(client.sin_port));
		} else {
			puts("Sent 14 bytes of data to client.");
		}
		if(closesocket(clientfd) == 0) {
			clientfd = 0;
			break;
		}
	}
	if(clientfd == 0) {
		printf("Client [%s:%d] disconnected!\n",
			inet_ntoa(client.sin_addr),ntohs(client.sin_port));
	}
	closesocket(sock);
	WSACleanup();
	return 0;

error:
	if(clientfd > 0)
		closesocket(clientfd);
	closesocket(sock);
	WSACleanup();
	return 1;
}
