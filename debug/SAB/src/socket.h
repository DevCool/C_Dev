#ifndef socket_h
#define socket_h

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

int create_socket(const char *hostname, int port, int *clientfd, struct sockaddr_in *clientaddr);
void close_socket(int *sockfd);
int handle_server(int *sockfd, int *clientfd, struct sockaddr_in *client, const char *filename,
		  int (*hdl_client)(int *sockfd, struct sockaddr_in *client,
				    const char *filename));
int handle_client(int *sockfd, struct sockaddr_in *client, const char *filename);

#endif
