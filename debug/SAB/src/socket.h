#ifndef socket_h
#define socket_h

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int create_socket(int (*hdl_client)(int *, struct sockaddr_in *));
void close_socket(int *sockfd);
int handle_client(int *sockfd, struct sockaddr_in *client);

#endif
