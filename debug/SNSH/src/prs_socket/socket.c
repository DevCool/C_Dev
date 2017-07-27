/*****************************************************************
 * socket.c - this handles creating, handling, and destroying of *
 *            sockets.                                           *
 *****************************************************************
 * Created by Philip "5n4k3" Simonson             (2017)         *
 *****************************************************************
 */

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* my includes */
#include "socket.h"
#include "../../../debug.h"

/* defines for main server */
#define MAX_BUFLEN 1024
#define BACKLOG 10
#define PORT 8888

/* socket_init() - initialize my socket functions.
 */
int socket_init(sockcreate_t init, sockcreate_func_t *socket_func) {
  sockcreate_func_t sock_func;
  
  switch(init) {
  case SOCKET_BIND:
    sock_func.socket_bind = &create_bind;
    break;
  case SOCKET_CONN:
    sock_func.socket_conn = &create_conn;
    break;
  default:
    return -1;
  }
  *socket_func = sock_func;
  return 0;
}

/* create_conn() - creates a socket, binds to a specified port,
 * and returns the sock file descriptor.
 */
int create_conn(const char *hostname, int port, int *serverfd, struct sockaddr_in *serveraddr) {
  struct sockaddr_in serv;
  int sockfd;

  memset(&serv, 0, sizeof(serv));
  serv.sin_family = AF_INET;
  if(port <= 0)
    serv.sin_port = htons(PORT);
  else
    serv.sin_port = htons(port);
  serv.sin_addr.s_addr = inet_addr(hostname);

  ERROR_FIXED((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0, "Cannot create socket.");
  ERROR_FIXED(connect(sockfd, (struct sockaddr *)&serv, sizeof serv) < 0,
	      "Could not connec to server.");
  *serveraddr = serv;
  *serverfd = sockfd;
  return sockfd;	/* returns 0 for success */

 error:
  return -1;
}

/* create_bind() - creates a socket, binds to a specified port,
 * and returns the sock file descriptor.
 */
int create_bind(const char *hostname, int port, int *clientfd, struct sockaddr_in *clientaddr) {
  struct sockaddr_in serv, client;
  socklen_t clientlen;
  int sockfd, newfd;
  int yes = 1;

  memset(&serv, 0, sizeof(serv));
  serv.sin_family = AF_INET;
  if(port <= 0)
    serv.sin_port = htons(PORT);
  else
    serv.sin_port = htons(port);
  serv.sin_addr.s_addr = inet_addr(hostname);

  ERROR_FIXED((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0, "Cannot create socket.\n");
  ERROR_FIXED(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1,
	      "Cannot set socket options.");
  ERROR_FIXED(bind(sockfd, (struct sockaddr *)&serv, sizeof(serv)) < 0,
	      "Cannot bind port to socket.");
  ERROR_FIXED(listen(sockfd, BACKLOG) < 0, "Cannot listen on socket.");
  ERROR_FIXED((newfd = accept(sockfd, (struct sockaddr *)&client, &clientlen)) < 0,
	      "Cannot accept connection.");
  *clientaddr = client;
  *clientfd = newfd;
  return sockfd;	/* returns 0 for success */

 error:
  return -1;
}

/* close_socket() - closes the socket file descriptor.
 */
void close_socket(int *sockfd) {
  if(sockfd)
    close(*sockfd);
}

/* handle_server() - handles the main server.
 */
int handle_server(int *sockfd, int *clientfd, struct sockaddr_in *client, const char *filename,
		  int (*hdl_client)(int *sockfd, struct sockaddr_in *client,
				    const char *filename)) {
  int retval;

  ERROR_FIXED(sockfd == NULL || clientfd == NULL || client == NULL, "Server not setup properly!");
  if((*hdl_client) == NULL)
    retval = handle_client(clientfd, client, filename);
  else
    retval = (*hdl_client)(clientfd, client, filename);
  return retval; /* return success */

error:
  return -1;
}

/* handle_client() - handles the main client.
 */
int handle_client(int *sockfd, struct sockaddr_in *client, const char *filename) {
  char buf[MAX_BUFLEN];
  if(filename == NULL)
    puts("Filename is NULL");
  memset(buf, 0, sizeof buf);
  snprintf(buf, sizeof buf, "Client: Connected from %s\n", inet_ntoa(client->sin_addr));
  puts(buf);
  memset(buf, 0, sizeof buf);
  snprintf(buf, sizeof buf, "Server: Hello client %s\n", inet_ntoa(client->sin_addr));
  ERROR_FIXED(send(*sockfd, buf, strlen(buf), 0) < 0, "Could not send data to client.");
  close_socket(sockfd);
  return 0; /* return success */

error:
  close_socket(sockfd);
  return -1;
}

#if defined(_WIN32) || (_WIN64)
/* DLL Main Function for windows */
int WINAPI DllMain(HANDLE hModule, DWORD fdwReason, LPVOID lpReserved) {
	/* Handle process attach */
	switch(fdwReason) {
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}

	return TRUE;
}
#endif
