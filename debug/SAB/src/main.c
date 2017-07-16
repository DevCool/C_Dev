/**************************************************************************
 * main.c - main program is right in this file, though it doesn't contain *
 *          much. but you should look at it anyway, learning from this    *
 *          project is what I want anyone (who wants to learn) to be able *
 *          to learn from this. That's what I made it for.                *
 **************************************************************************
 * Created by Philip "5n4k3" Simonson                (2017)               *
 **************************************************************************
 */

/* standard headers */
#include <stdio.h>
#include <string.h>
#include "socket.h"
#include "helper.h"

/* function pointer to handle clients */
int hdl_client(int *sockfd, struct sockaddr_in *client, const char *filename);

/* main() - entry point for program.
 */
int main(int argc, char *argv[]) {
  struct sockaddr_in client;
  int sockfd, clientfd, retval;

  if(argc != 2) {
    printf("Usage: %s <ip-address>\n", argv[0]);
    return 1;
  }

  sockfd = create_socket(argv[1], 0, &clientfd, &client);
  retval = handle_server(&sockfd, &clientfd, &client, NULL, &hdl_client);
  close_socket(&sockfd);
  
  return retval;
}

/* hdl_client() - handles connect client.
 */
int hdl_client(int *sockfd, struct sockaddr_in *client, const char *filename) {
  if(filename == NULL) {}
  cmd_loop(sockfd, client);
  close_socket(sockfd);
  return 0; /* return success */
}
