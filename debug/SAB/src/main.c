#include <stdio.h>
#include "socket.h"

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

int hdl_client(int *sockfd, struct sockaddr_in *client) {
  puts("Got here!");
  return 0; /* return success */
}
