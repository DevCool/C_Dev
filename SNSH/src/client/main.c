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
    setvbuf(stdin, NULL, _IONBF, 0);
    ERROR_FIXED(handle_server(&sockfd, &clientfd, &client, NULL, &hdl_client) < 0,
		"Could not handle server.\n");
    retval = hdl_client(&sockfd, &client, NULL);
  } else {
    ERROR_FIXED(socket_init(SOCKET_CONN, &sockfunc) < 0, "Socket init failed.\n");
    ERROR_FIXED((sockfd = create_conn(argv[1], atoi(argv[2]), &clientfd, &client)) < 0,
		"Could not create socket.\n");
    setvbuf(stdin, NULL, _IONBF, 0);
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

#define DATALEN 16384

int find_network_newline(char *msg, int total) {
  int i;
  for(i = 0; i < total; i++)
    if(msg[i] == '\n')
      return i;
  return -1;
}

static int inbuf;

int getline_network(char *msg) {
  int bytes_read = read(STDIN_FILENO, msg, 256-inbuf);
  short flag = -1;
  int where = 0;

  inbuf += bytes_read;
  where = find_network_newline(msg, bytes_read);
  if(where >= 0) {
    /*    char nch = '\0';
	  memcpy(msg+where, &nch, 1); */
    inbuf = 0;
    flag = 0;
  }
  return flag;
}

int hdl_client(int *sockfd, struct sockaddr_in *client, const char *filename) {
#if defined(_WIN32) || (_WIN64)
  FD_SET rfds;
#else
  fd_set rfds;
#endif
  struct timeval tv;
  char msg[DATALEN];
  char buf[BUFSIZ];
  unsigned int bytes;
  int ret;

  while(1) {
    FD_ZERO(&rfds);
    tv.tv_sec = 0;
    tv.tv_usec = 30;
    FD_SET(*sockfd, &rfds);
    FD_SET(STDIN_FILENO, &rfds);
    ret = select(*sockfd + 1, &rfds, NULL, NULL, &tv);

    if(ret < 0)
      break;

    if(FD_ISSET(*sockfd, &rfds)) {
      memset(msg, 0, sizeof msg);
      ERROR_FIXED((ret = recv(*sockfd, msg, sizeof msg, 0)) < 0, "Could not recv data.\n");
      if(ret == 0) {
	puts("Connection closed.");
	break;
      } else {
	printf("%s", msg);
      }
    }
    if(FD_ISSET(STDIN_FILENO, &rfds)) {
      memset(buf, 0, sizeof buf);
      ERROR_FIXED(getline_network(buf) < 0, "Could not get input.\n");
      ERROR_FIXED((bytes = send(*sockfd, buf, strlen(buf), 0)) != strlen(buf),
		  "Could not send data.\n");
      if(bytes == 0) {
	puts("Connection closed.");
	break;
      }
    }
  }
  close_socket(sockfd);
  return 0;

 error:
  close_socket(sockfd);
  return 1;
}
