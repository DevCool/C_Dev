#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "socket.h"
#include "../../debug.h"

#define MAX_BUFLEN 1024
#define BACKLOG 10
#define PORT 8888

int create_socket(int (*hdl_client)(int *, struct sockaddr_in *)) {
  struct addrinfo hints, *servinfo = NULL, *p = NULL;
  struct sockaddr_storage addr;
  socklen_t addrlen;
  char buf[MAX_BUFLEN];
  int sockfd, newconn;
  int yes = 1;
  
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  ERROR_FIXED(getaddrinfo(NULL, PORT, &hints, &servinfo) == 0, "Cannot get addr info.");
  for(p = servinfo; p != NULL; p = p->ai_next) {
    if((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      printf("server: Cannot create socket.");
      continue;
    }
    ERROR_FIXED(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
			   sizeof(int)) == -1, "Cannot set socket options.");
    ERROR_FIXED(bind(sockfd, p->ai_addr, p->ai_addrlen) < 0, "Cannot bind to socket.");
    break;
  }
  freeaddrinfo(servinfo);

  ERROR_FIXED(listen(sockfd, BACKLOG) < 0, "Cannot listen on socket.");
  
  while(1) {
    ERROR_FIXED((newconn = accept(sockfd, &addr, &addrlen)) < 0,
		"Failed to receive connection.");
    if(newconn > 0) {
      memset(buf, 0, sizeof(buf));
      snprintf(buf, sizeof(buf), "Connection opened to %s\n", inet_ntoa(((struct sockaddr_in *)&addr)->sin_addr));
      puts(buf);
      ERROR_FIXED(send(newconn, buf, strlen(buf), 0) < 0, "Cannot send data.");
      if((*hdl_client) == NULL)
	handle_client(&newconn, ((struct sockaddr_in *)&addr));
      else
	(*hdl_client)(&newconn, ((struct sockaddr_in *)&addr));
    } else {
      puts("Connection closed by remote host.");
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
  
  return 0; /* return success */
}
