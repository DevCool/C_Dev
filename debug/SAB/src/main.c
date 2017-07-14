#include <stdio.h>
#include "socket.h"

int main(int argc, char *argv[]) {
  int sockfd;

  if(argc != 2) {
    printf("Usage: %s <ip-address>\n", argv[0]);
    return 1;
  }

  sockfd = create_socket(argv[1], NULL);
  close_socket(&sockfd);
  
  return 0;
}
