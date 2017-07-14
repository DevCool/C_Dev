#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "socket.h"
#include "debug.h"

#define MAX_BUFLEN 1024
#define BACKLOG 10
#define PORT 8888

int create_socket(const char *hostname, int (*hdl_client)(int *, struct sockaddr_in *)) {
  struct sockaddr_in serv, addr;
  socklen_t addrlen;
  char buf[MAX_BUFLEN];
  int sockfd, newconn;
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
  
  while(1) {
    ERROR_FIXED((newconn = accept(sockfd, (struct sockaddr *)&addr, &addrlen)) < 0,
		"Failed to receive connection.");
    if(newconn > 0) {
      memset(buf, 0, sizeof(buf));
      snprintf(buf, sizeof(buf), "Connection opened to %s\n", inet_ntoa(addr.sin_addr));
      puts(buf);
      ERROR_FIXED(send(newconn, buf, strlen(buf), 0) < 0, "Cannot send data.");
      if((*hdl_client) == NULL)
	handle_client(&newconn, &addr);
      else
	(*hdl_client)(&newconn, &addr);
    } else {
      puts("Connection closed by remote host.");
      break;
    }
  }

  close_socket(&sockfd);
  return 0;	/* returns 0 for success */

 error:
  close_socket(&sockfd);
  return -1;
}

void close_socket(int *sockfd) {
  if(sockfd)
    close(*sockfd);
}

int handle_client(int *sockfd, struct sockaddr_in *client) {
  char buf[MAX_BUFLEN];

  memset(buf, 0, sizeof buf);
  snprintf(buf, sizeof buf, "Client: Connected from %s\n", inet_ntoa(client->sin_addr));
  ERROR_FIXED(send(*sockfd, buf, strlen(buf), 0) < 0, "Could not send data to server.");
  close_socket(sockfd);
  
  return 0; /* return success */

error:
  return -1;
}
