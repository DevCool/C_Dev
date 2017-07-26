#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#define MAX_PASS 25
#define MAX_CLIENTS 5

#define SEX_DATA "      _ _\r\n"\
  "     / | \\\r\n"\
  "     | | |\r\n"\
  "8======| |\r\n"\
  "     \\_|_/\r\n\r\n"

#define MSGIP(M, X) printf("[INFO] : " M " [%s]!\n", inet_ntoa(X))

int main(int argc, char **argv) {
  struct addrinfo hints, *server, *p;
  struct sockaddr client[MAX_CLIENTS];
  socklen_t addrlen[MAX_CLIENTS];
  int sockfd, clientfd[MAX_CLIENTS], yes = 1;
  int i, pid = 0, done;
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

    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, p->ai_addrlen) < 0) {
      fprintf(stderr, "Error: Failed to set socket options.\n");
      return 2;
    }
    if(bind(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
      fprintf(stderr, "Error: Failed to bind port to socket.\n");
      return 1;
    }
    break;
  }

  freeaddrinfo(server);
  listen(sockfd, MAX_CLIENTS);
  done = 0;
  if((pid = fork()) < 0) {
    fprintf(stderr, "Error: Could not fork to background.\n");
    close(sockfd);
    return 3;
  } else if(pid == 0) {
    while(!done) {
      i = 0;
      while(i < MAX_CLIENTS) {
	if((clientfd[i] = accept(sockfd, &client[i], &addrlen[i])) < 0) {
	  fprintf(stderr, "Error: Cannot accept connect from client.\n");
	  return 2;
	} else {
	  MSGIP("Client connected", ((struct sockaddr_in*)&client[i])->sin_addr);
	  if(send(clientfd[i], SEX_DATA, strlen(SEX_DATA), 0) != strlen(SEX_DATA))
	    goto error;

	  snprintf(data, sizeof data, "Too deep.\r\n");
	  if(send(clientfd[i], data, strlen(data), 0) != strlen(data))
	    goto error;
	  if(close(clientfd[i]) == 0)
	    MSGIP("Client disconnected", ((struct sockaddr_in*)&client[i])->sin_addr);
	  ++i;
	}
      }
    }
  }
  close(sockfd);
  return 0;

 error:
  for(i = 0; i < MAX_CLIENTS; i++)
    close(clientfd[i]);
  close(sockfd);
  return 254;
}
