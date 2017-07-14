#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "socket.h"
#include "../../debug.h"

#define MAX_BUFLEN 1024
#define BACKLOG 10
#define PORT 8888

int create_socket(const char *hostname, int *clientfd, struct sockaddr_in *clientaddr) {
  struct sockaddr_in serv, client;
  socklen_t clientlen;
  int sockfd, newfd;
  int yes = 1;

  memset(&serv, 0, sizeof(serv));
  serv.sin_family = AF_INET;
  serv.sin_port = htons(PORT);
  serv.sin_addr.s_addr = inet_addr(hostname);

  ERROR_FIXED((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0, "Cannot create socket.");
  ERROR_FIXED(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1,
	      "Cannot set socket options.");
  ERROR_FIXED(bind(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0,
	      "Cannot bind port to socket.");
  ERROR_FIXED(listen(sockfd, BACKLOG) < 0, "Cannot listen on socket.");
  ERROR_FIXED((newfd = accept(sockfd, (struct sockaddr *)&client, &clientlen)) < 0,
	      "Cannot accept connection.");
  *clientaddr = client;
  *clientfd = newfd;
  return sockfd;	/* returns 0 for success */

 error:
  return -1;
}

void close_socket(int *sockfd) {
  if(sockfd)
    close(*sockfd);
}

int handle_server(int *sockfd, int *clientfd, struct sockaddr_in *client,
		  int (*hdl_client)(int *sockfd, struct sockaddr_in *client)) {
  int retval;

  ERROR_FIXED(sockfd == NULL || clientfd == NULL || client == NULL, "Server not setup properly!");
  if((*hdl_client) == NULL)
    retval = handle_client(clientfd, client);
  else
    retval = (*hdl_client)(clientfd, client);
  return retval; /* return success */

error:
  return -1;
}

int handle_client(int *sockfd, struct sockaddr_in *client) {
  char buf[MAX_BUFLEN];
  memset(buf, 0, sizeof buf);
  snprintf(buf, sizeof buf, "Client: Connected from %s\n", inet_ntoa(client->sin_addr));
  puts(buf);
  memset(buf, 0, sizeof buf);
  snprintf(buf, sizeof buf, "Server: Hello client %s\n", inet_ntoa(client->sin_addr));
  ERROR_FIXED(send(*sockfd, buf, strlen(buf), 0) < 0, "Could not send data to client.");
  close_socket(sockfd);
  return 0; /* return success */

error:
  close_socket(sockfd);
  return -1;
}
