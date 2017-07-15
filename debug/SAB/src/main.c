#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "helper.h"

int hdl_client(int *sockfd, struct sockaddr_in *client);

int main(int argc, char *argv[]) {
  struct sockaddr_in client;
  int sockfd, clientfd, retval;

  if(argc != 2) {
    printf("Usage: %s <ip-address>\n", argv[0]);
    return 1;
  }

  sockfd = create_socket(argv[1], &clientfd, &client);
  retval = handle_server(&sockfd, &clientfd, &client, &hdl_client);
  close_socket(&sockfd);
  
  return retval;
}

#define DATALEN 1024
#define RECVLEN 512

int hdl_client(int *sockfd, struct sockaddr_in *client) {
  cmd_loop(sockfd, client);
  close_socket(sockfd);
  return 0; /* return success */
}
