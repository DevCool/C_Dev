/********************************************
 * Simple Telnet Client
 * by Philip "5n4k3" Simonson
 ********************************************
 */

#include <winsock2.h>
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define DO 0xfd
#define WONT 0xfc
#define WILL 0xfb
#define DONT 0xfe
#define CMD 0xff
#define CMD_ECHO 1
#define CMD_WINDOW_SIZE 31

void negotiate(sock,buf,len)
	int sock;
	unsigned char *buf;
	int len;
{
	int i;

	if(buf[1]==DO && buf[2]==CMD_WINDOW_SIZE) {
		unsigned char tmp1[10]={255,251,31};
		if(send(sock,tmp1,3,0)<0)
			exit(1);

		unsigned char tmp2[10]={255,250,31,0,80,0,24,255,240};
		if(send(sock,tmp2,9,0)<0)
			exit(1);
		return;
	}

	for(i=0; i<len; ++i)
		if(buf[i]==DO)
			buf[i]=WONT;
		else if(buf[i]==WILL)
			buf[i]=DO;
	if(send(sock,buf,len,0)<0)
		exit(1);
}

#define BUFLEN 20
int main(argc,argv)
	int argc;
	char *argv[];
{
	WSADATA wsaData;
	int sock;
	struct sockaddr_in server;
	unsigned char buf[BUFLEN+1];
	int len;
	int i;

	if(argc<2 || argc>3) {
		printf("Usage: %s address [port]\n",argv[0]);
		return 1;
	}
	int port = 23;
	if(argc==3)
		port = atoi(argv[2]);

	/* Initialize winsock */
	WSAStartup(0x0202,&wsaData);

	/* Create socket */
	if((sock=socket(AF_INET,SOCK_STREAM,0)) == INVALID_SOCKET) {
		perror("Could not create socket. Error");
		WSACleanup();
		return 1;
	}

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(argv[1]);

	/* Connect to remote machine */
	if(connect(sock,(struct sockaddr*)&server,sizeof(server)) == INVALID_SOCKET) {
		perror("connection failed. Error");
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	puts("Connected...\n");

	struct timeval ts;
	ts.tv_sec = 1; /* 1 second */
	ts.tv_usec = 0;

	while(1) {
		/* select setup */
		FD_SET rd;
		FD_ZERO(&rd);
		if(sock!=0) {
			FD_SET(sock,&rd);
		}
		FD_SET(0,&rd);

		/* wait for data */
		int nready = select(sock+1,&rd,NULL,NULL,&ts);
		if(nready<SOCKET_ERROR) {
			perror("select. Error");
			goto error;
		} else if(nready==0) {
			ts.tv_sec = 1; /* 1 second */
			ts.tv_usec = 0;
		} else if(sock!=0 && FD_ISSET(sock,&rd)) {
			/* start by reading a single byte */
			int rv;
			if((rv=recv(sock,buf,1,0))<0)
				goto error;
			else if(rv==0) {
				printf("Connection closed by the remote host.\n\r");
				return 0;
			}

			if(buf[0]==CMD) {
				/* read 2 more bytes */
				len=recv(sock,buf+1,2,0);
				if(len<0)
					goto error;
				else if(len==0) {
					printf("Connection close by the remote host.\n\r");
					return 0;
				}
				negotiate(sock,buf,3);
			} else {
				len = 1;
				buf[len]='\0';
				printf("%s",buf);
				fflush(stdin);
			}
		} else if(FD_ISSET(0,&rd)) {
			buf[0]=getc(stdin); /* fgets(buf, 1, stdin); */
			if(send(sock,buf,1,0)<0)
				goto error;
			if(buf[0]=='\n') /* with the terminal in raw mode we need to put lF */
				putchar('\r');
		}
	}

	closesocket(sock);
	WSACleanup();
	return 0;

error:
	closesocket(sock);
	WSACleanup();
	return 1;
}
