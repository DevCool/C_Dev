#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../../../debug.h"
#include "../socket.h"
#include "../transfer.h"

#define UPLOAD_PORT 30587
#define DOWNLOAD_PORT 30588

int handle_download(int *sockfd, struct sockaddr_in *client, const char *filename);
int handle_upload(int *sockfd, struct sockaddr_in *client, const char *filename);

int main(int argc, char *argv[]) {
  sockcreate_func_t sock_func;
  struct sockaddr_in client;
  int sockfd, clientfd, retval;

  if(argc < 3 || argc > 4) {
    printf("Usage: %s -[ud] <ipaddress> <filename>\n", argv[0]);
    return 1;
  }

  if(argc == 3) {
    socket_init(SOCKET_CONN, &sock_func);
    sockfd = sock_func.socket_conn(argv[2], UPLOAD_PORT, &clientfd, &client);
    retval = handle_server(&sockfd, &clientfd, &client, argv[2], &handle_upload);
    close_socket(&sockfd);
  } else {
    if(argv[1][0] == '-') {
      switch(argv[1][1]) {
      case 'd':
	socket_init(SOCKET_CONN, &sock_func);
	sockfd = sock_func.socket_conn(argv[2], DOWNLOAD_PORT, &clientfd, &client);
	retval = handle_server(&sockfd, &clientfd, &client, argv[3], &handle_download);
	close_socket(&sockfd);
	break;
      case 'u':
	socket_init(SOCKET_CONN, &sock_func);
	sockfd = sock_func.socket_conn(argv[2], UPLOAD_PORT, &clientfd, &client);
	retval = handle_server(&sockfd, &clientfd, &client, argv[3], &handle_upload);
	close_socket(&sockfd);
	break;
      default:
	printf("Unknown option '%c'\n", argv[1][1]);
      }
    } else {
      printf("Unknown switch: %s\n", argv[1]);
      return 3;
    }
  }
  return retval;
}

int handle_download(int *sockfd, struct sockaddr_in *client, const char *filename) {
  if(sockfd == NULL || client == NULL || filename == NULL)
    return -1;
  
  return 0; /* return success */
}

int handle_upload(int *sockfd, struct sockaddr_in *client, const char *filename) {
  if(sockfd == NULL || client == NULL || filename == NULL)
    return -1;
  
  return 0; /* return success */
}
