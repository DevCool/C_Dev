/******************************************
 * S.A.B. Client
 ******************************************
 * by Philip "5n4k3" Simonson
 ******************************************
 */

#if defined(_WIN32) || (_WIN64)
	#include <winsock2.h>
	#include <windows.h>
#else
	#include <sys/socket.h>
	#include <arpa/inet.h>
#endif

#define MY_PORT 4440
#define MY_ADDR "127.0.0.1"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <unistd.h>

struct DATA {
	SOCKET serverfd;
	struct sockaddr_in server;
	struct sockaddr_in client;
	WSADATA wsaData;
	char receiving;
};

int pstricmp(s1,s2)
	char *const s1;
	char *const s2;
{
	char *tmp;
	char *tmp2;

	tmp = s1;
	tmp2 = s2;
	while(*tmp && tmp != s1+1 && *tmp2 && tmp2 != s2+1)
		if(tolower(*tmp) != tolower(*tmp2))
			return *tmp-*tmp2;
		else {
			++tmp;
			++tmp2;
		}
	return 0;
}

/* handle_sockets() - handle server/client sockets.
 */
void handle_sockets(data)
	struct DATA data;
{
	/* Todo: Add code here */
}

/* setup_socket() - setup the sockets and connect.
 */
int setup_sockets(data,address)
	struct DATA *data;
	const char *address;
{
#if defined(_WIN32) || (_WIN64)
	ZeroMemory(data,sizeof(data));
	ZeroMemory(&data->wsaData,sizeof(data->wsaData));
	ZeroMemory(&data->server,sizeof(data->server));
	ZeroMemory(&data->client,sizeof(data->client));
	WSAStartup(MAKEWORD(2,2),&data->wsaData);

	if((data->serverfd = WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,NULL,0,0)) == INVALID_SOCKET) {
		fprintf(stderr,"Error code: %8X\n",WSAGetLastError());
		WSACleanup();
		return 1;
	}

	data->server.sin_family = AF_INET;
	data->server.sin_port = htons(MY_PORT);
	data->server.sin_addr.s_addr = inet_addr(address);

	if(connect(data->serverfd,(struct sockaddr*)&data->server,sizeof(data->server)) == INVALID_SOCKET) {
		fprintf(stderr,"Error code: %8X\n",WSAGetLastError());
		closesocket(data->serverfd);
		WSACleanup();
		return 1;
	}
#else
	bzero(data,sizeof(data));
	bzero(&data->server,sizeof(data->server));
	bzero(&data->client,sizeof(data->client));

	if((data->serverfd = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
		fprintf(stderr,"Error: %s\n",strerror(errno));
		return 1;
	}

	server->sin_family = AF_INET;
	server->sin_port = htons(MY_PORT);
	server->sin_addr.s_addr = inet_addr(address);

	if(connect(data->serverfd,(struct sockaddr*)&data->client,sizeof(data->client)) < 0) {
		fprintf(stderr,"Error: %s\n",strerror(errno));
		close(data->serverfd);
		return 1;
	}
#endif
	return 0;
}

/* cleanup() - clean up everything used in this
 *             program.
 */
void cleanup(data)
	struct DATA *data;
{
#if defined(_WIN32) || (_WIN64)
	if(data->serverfd != INVALID_SOCKET)
		closesocket(data->serverfd);
	WSACleanup();
#else
	if(data->serverfd >= 0) {
		close(data->serverfd);
#endif
}

/* main() - entry point for program.
 */
int main(argc,argv)
	int argc;
	char **argv;
{
	int read_data();
	int write_data();
	struct DATA data;
	int bytes;

	if(argc > 2) {
		fprintf(stderr,"Usage: %s [IPAddress]\n",argv[0]);
		return 0;
	}
	if(argc == 1) {
		if(setup_sockets(&data,MY_ADDR) == 1) {
			return 1;
		}
	} else {
		if(setup_sockets(&data,argv[1]) == 1) {
			return 1;
		}
	}

	bytes = 0;
	while(1) {
		bytes = read_data(&data);
		if(bytes < 0) break;
		if(write_data(&data) == 1) break;
	}

	if(bytes == 0) {
		puts("You were disconnect.\n");
	}
	cleanup(&data);
	return 0;
}

int get_line(buf,size)
	char buf[];
	size_t size;
{
	int c,i,j;

	j = 0;
	memset(buf,0,size);
	for(i=0; (c = getchar()) != EOF && c != 0x0D; ++i)
		if(i < size-2) {
			buf[i] = c;
			++j;
		}
	if(c == 0x0D) {
		buf[i] = c;
		++i;
		++j;
	}
	++j;
	buf[j] = 0;
	return i;
}

#define CHUNK_SIZE 512
#define MAXBUF 128

int read_data(data)
	struct DATA *data;
{
	int size_recv,total_size;
	char chunk[CHUNK_SIZE];

	/* loop receive */
	total_size = 0;
	while(1) {
		memset(chunk,0,CHUNK_SIZE);
		if((size_recv = recv(data->serverfd,chunk,CHUNK_SIZE,0)) < 0) {
			break;
		} else {
			total_size += size_recv;
			chunk[size_recv] = 0;
			printf("%s", chunk);
		}
	}
	return total_size;
}

int write_data(data)
	struct DATA *data;
{
	char buf[MAXBUF];
	int bytes;

	bytes = 0;
	memset(buf,0,MAXBUF);
	while(get_line(buf,MAXBUF) > 0) {
		send(data->serverfd,buf,bytes,0);
		memset(buf,0,MAXBUF);
	}

	if(bytes == 0) {
		fprintf(stdout,"Server terminated connection!\n");
		return 1;
	}

	return 0;
}
