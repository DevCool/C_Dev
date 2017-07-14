#ifndef socket_h
#define socket_h

#define _XOPEN_SOURCE 600

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int create_socket(const char *hostname, int (*hdl_server)(int *));
void close_socket(int *sockfd);
int handle_server(int *sockfd);

#endif
