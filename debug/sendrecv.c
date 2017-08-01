#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#if defined(_WIN32) || (_WIN64)
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#elif __linux
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SOCKET_ERROR (-1)
#endif

int sendall(int sd, char *s, int *len) {
  int total = 0;
  int bytesleft = *len;
  int n;

  while(total < *len) {
    n = send(sd, s+total, bytesleft, 0);
    if(n == -1)
      break;
    total += n;
    bytesleft -= n;
  }

  *len = total; /* return total bytes */
  return (n == -1) ? -1 : 0; /* return -1 on failure, 0 success. */
}

int recvall(int sd, char *s, int *len) {
  int total = 0;
  int bytesleft = strlen(s);
  int n;

  while(total < *len) {
    n = recv(sd, s+total, bytesleft, 0);
    if(n == -1 || *(s+total) == '\n' || *(s+total) == '\r')
      break;
    total += n;
    bytesleft -= n;
  }

  *len = total; /* return total bytes received */
  s[total] = '\0'; /* terminate string with null character. */
  return (n == -1) ? -1 : 0; /* return -1 on failure, 0 success. */
}

#define MAXLEN 128

int main(void) {
  struct addrinfo hints, *res;
  struct sockaddr client;
  socklen_t addrlen = sizeof(client);
  int len, rlen, sockfd, newfd, rv;
  char prompt[] = "Enter password: ";
  char entry[MAXLEN];
#if defined(_WIN32) || (_WIN64)
  WSADATA wsaData;
  u_long on = 1;

  if(WSAStartup(0x0202, &wsaData) != 0) {
    MessageBox(NULL, "There was an error!", "Error", MB_OK | MB_ICONERROR);
    exit(EXIT_FAILURE);
  }
#else
  int on = 1;
#endif
  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if((rv = getaddrinfo(NULL, "5555", &hints, &res)) == SOCKET_ERROR) {
    perror("getaddrinfo");
    exit(EXIT_FAILURE);
  }
  if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
#if defined(_WIN32) || (_WIN64)
  if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&on, sizeof(int)) == SOCKET_ERROR) {
#else
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) == SOCKET_ERROR) {
#endif
      perror("setsockopt");
      close(sockfd);
      exit(EXIT_FAILURE);
    }
#if defined(_WIN32) || (_WIN64)
    if(ioctlsocket(sockfd, FIONBIO, &on) != 0) {
      perror("ioctlsocket");
      close(sockfd);
      exit(EXIT_FAILURE);
    }
#else
    if(fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
      perror("fcntl");
      close(sockfd);
      exit(EXIT_FAILURE);
    }
#endif
    if(bind(sockfd, res->ai_addr, res->ai_addrlen) == SOCKET_ERROR) {
      perror("bind");
      goto error;
    }

    listen(sockfd, 0);
    if((newfd = accept(sockfd, &client, &addrlen)) == SOCKET_ERROR) {
      perror("accept");
      goto error;
    }

    do {
      len = strlen(prompt);
      rlen = sizeof(entry);
      memset(entry, 0, sizeof entry);
      if(sendall(newfd, prompt, &len) == SOCKET_ERROR) {
	perror("sendall");
	printf("Only sent %d bytes, because of error.\n", len);
      }
      if(recvall(newfd, entry, &rlen) == SOCKET_ERROR) {
	perror("recvall");
	printf("Only received %d bytes, because of error.\n", rlen);
      }
    } while(strncmp(entry, "testPass", sizeof entry) != 0);

#if defined(_WIN32) || (_WIN64)
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
    return 0;

  error:
#if defined(_WIN32) || (_WIN64)
    closesocket(sockfd);
    WSACleanup();
#else
    close(sockfd);
#endif
    return 1;
  }
