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
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "prs_socket/socket.h"
#include "helper.h"
#include "debug.h"
#include "snshimg.h"

/* function pointer to handle clients */
int hdl_client(int *sockfd, struct sockaddr_in *client, const char *filename);

/* main() - entry point for program.
 */
int main(int argc, char *argv[]) {
  sockcreate_func_t sock_funcs;
  struct sockaddr_in client;
  int sockfd, clientfd, retval;

  if(argc != 2) {
    printf("Usage: %s <ip-address>\n", argv[0]);
    return 1;
  }

  ERROR_FIXED(socket_init(SOCKET_BIND, &sock_funcs) < 0, "Could not initialize socket funcs.");
  sockfd = sock_funcs.socket_bind(argv[1], 0, &clientfd, &client);
  retval = handle_server(&sockfd, &clientfd, &client, NULL, &hdl_client);
  close_socket(&sockfd);
  return retval;

 error:
  close_socket(&sockfd);
  return -1;
}

/* hdl_client() - handles connect client.
 */
int hdl_client(int *sockfd, struct sockaddr_in *client, const char *filename) {
  char msg[256];
  char password[9];
  char entry[9];
  if(filename == NULL) {}
  ERROR_FIXED(send(*sockfd, SNSH_IMGDATA, strlen(SNSH_IMGDATA), 0)
		!= strlen(SNSH_IMGDATA), "Could not send all img data.\n");
  memset(password, 0, sizeof(password));
  snprintf(password, sizeof(password), "AW96B6\n");
  do {
    memset(msg, 0, sizeof msg);
    memset(entry, 0, sizeof entry);
    snprintf(msg, sizeof msg, "Enter password: ");
    ERROR_FIXED(send(*sockfd, msg, strlen(msg), 0) != (int)strlen(msg),
		"Could not send data to client.\n");
    ERROR_FIXED(recv(*sockfd, entry, sizeof(entry), 0) < 0, "Could not recv from client.\n");
    ERROR_FIXED(strncmp(entry, "exit\r\n", sizeof(entry)) == 0, "You've quit SAB.\n");
  } while(strncmp(entry, password, sizeof(entry)) != 0);
  cmd_loop(sockfd, client);
  close_socket(sockfd);
  return 0; /* return success */

 error:
  close_socket(sockfd);
  return 1;
}
