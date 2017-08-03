/*********************************************************
 * main.c - SNSH Client main source file.
 *********************************************************
 * Created by Philip "5n4k3" Simonson
 *********************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../debug.h"
#include "../prs_socket/socket.h"

int hdl_client(int *, struct sockaddr_in *, const char *);

int main(int argc, char *argv[]) {
  sockcreate_func_t sockfunc;
  struct sockaddr_in client;
  int sockfd, clientfd, retval;

  if(argc < 2 || argc > 3) {
    printf("Usage: %s <ipaddress> [port]\n", argv[0]);
    return 1;
  }

  if(argc == 2) {
    ERROR_FIXED(socket_init(SOCKET_CONN, &sockfunc) < 0, "socket init failed.\n");
    ERROR_FIXED((sockfd = create_conn(argv[1], 8888, &clientfd, &client)) < 0,
		"Could not create socket.\n");
    ERROR_FIXED(handle_server(&sockfd, &clientfd, &client, NULL, &hdl_client) < 0,
		"Could not handle server.\n");
    retval = hdl_client(&sockfd, &client, NULL);
  } else {
    ERROR_FIXED(socket_init(SOCKET_CONN, &sockfunc) < 0, "Socket init failed.\n");
    ERROR_FIXED((sockfd = create_conn(argv[1], atoi(argv[2]), &clientfd, &client)) < 0,
		"Could not create socket.\n");
    ERROR_FIXED(handle_server(&sockfd, &clientfd, &client, NULL, &hdl_client) < 0,
		"Could not handle server.\n");
    retval = hdl_client(&sockfd, &client, NULL);
  }
  close_socket(&sockfd);
  return retval;

 error:
  close_socket(&sockfd);
  return 1;
}

#define DATALEN 1024

int hdl_client(int *sockfd, struct sockaddr_in *client, const char *filename) {
#if defined(_WIN32) || (_WIN64)
  FD_SET rfds, wfds;
#else
  fd_set rfds, wfds;
#endif
  struct timeval tv;
  char msg[BUFSIZ];
  char buf[DATALEN];
  unsigned int bytes;
  int ret;
  
  while(1) {
    FD_ZERO(&rfds);
    FD_ZERO(&wfds);
    tv.tv_sec = 0;
    tv.tv_usec = 30;
    FD_SET(*sockfd, &rfds);
    FD_SET(*sockfd, &wfds);
    ret = select(*sockfd + 1, &rfds, &wfds, NULL, &tv);

    if(ret < 0)
      break;

    if(FD_ISSET(*sockfd, &rfds)) {
      memset(msg, 0, sizeof msg);
      ERROR_FIXED((ret = recv(*sockfd, msg, sizeof msg, 0)) < 0, "Could not recv data.\n");
      if(ret == 0) {
	puts("Connection closed.");
	break;
      }
      printf("%s", msg);
    }
    if(FD_ISSET(*sockfd, &wfds)) {
      memset(buf, 0, sizeof buf);
      ERROR_FIXED(fgets(buf, sizeof buf, stdin) == NULL, "Could not get input.\n");
      ERROR_FIXED((bytes = send(*sockfd, buf, strlen(buf), 0)) != strlen(buf),
		  "Could not send data.\n");
      if(bytes == 0) {
	puts("Connection closed.");
	break;
      }
    }
  }
  FD_ZERO(&rfds);
  FD_ZERO(&wfds);
  close_socket(sockfd);
  return 0;

 error:
  FD_ZERO(&rfds);
  FD_ZERO(&wfds);
  close_socket(sockfd);
  return 1;
}
