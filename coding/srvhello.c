#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_PASS 25

int main(int argc, char **argv) {
  struct addrinfo hints, *server, *p;
  struct sockaddr client;
  socklen_t addrlen;
  int sockfd, clientfd;
  char data[BUFSIZ];
  char password[MAX_PASS];

  memset(data, 0, sizeof data);
  memset(password, 0, sizeof password);
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if(getaddrinfo(NULL, "35970", &hints, &server) < 0) {
    fprintf(stderr, "Error: Cannot get addrinfo\n");
    return 1;
  }

  for(p = server; p != NULL; p = p->ai_next) {
    if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
      fprintf(stderr, "Error: Could not create socket.\n");
      continue;
    }

    if(bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
      fprintf(stderr, "Error: Failed to bind port to socket.\n");
      return 1;
    }
    break;
  }

  listen(sockfd, 0);
  if((clientfd = accept(sockfd, &client, &addrlen)) < 0) {
    fprintf(stderr, "Error: Cannot accept connect from client.\n");
    return 2;
  }

  snprintf(data, sizeof data, "Hello from the network.\r\n");
  if(send(clientfd, data, strlen(data), 0) != strlen(data))
    goto error;
  close(clientfd);
  close(sockfd);
  return 0;

 error:
  close(clientfd);
  close(sockfd);
  return 254;
}
