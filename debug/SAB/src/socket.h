#ifndef socket_h
#define socket_h

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int create_socket(const char *hostname);
void close_socket(int *sockfd);
int handle_server(int *sockfd, int (*hdl_client)(int *sockfd, struct sockaddr_in *client));
int handle_client(int *sockfd, struct sockaddr_in *client);

#endif
