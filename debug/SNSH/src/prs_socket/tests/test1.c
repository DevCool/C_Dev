#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../socket.h"

int main(void) {
  sockcreate_func_t sock_func;
  struct sockaddr_in client;
  int sockfd, clientfd, retval;

  /* initialize socket func */
  socket_init(SOCKET_BIND, &sock_func);
  /* bind socket to port */
  sockfd = sock_func.socket_bind("0.0.0.0", 5555, &clientfd, &client);
  /* handle server (swap to client handler) */
  retval = handle_server(&sockfd, &clientfd, &client, NULL, NULL);
  /* cleanup socket */
  close_socket(&sockfd);
  return retval;
}
