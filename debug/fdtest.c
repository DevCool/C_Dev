#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#if defined(_WIN32) || (_WIN64)
#include <ws2tcpip.h>
#include <windows.h>
#elif __linux
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

int main(void) {
  struct addrinfo hints, *res = NULL;
  struct sockaddr client;
  socklen_t addrlen;
  int sockfd, clientfd, rv, i;
  int fdmax = 0;
  char entry[9];
#ifdef __linux
  fd_set master, read_fds;
#elif _WIN32 || _WIN64
  FD_SET master, read_fds;
  u_long on = 1;
#endif
#if defined(_WIN32) || (_WIN64)
  WSADATA wsaData;
	
  if(WSAStartup(0x0202, &wsaData) != 0) {
    MessageBox(NULL, "Cannot start winsock library!", "Error!", MB_OK | MB_ICONERROR);
    return 1;
  }
#endif
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  if((rv = getaddrinfo(NULL, "5555", &hints, &res)) < 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
    return 1;
  }

  if((sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0) {
    perror("socket");
    goto error;
  }

#if defined(_WIN32) || (_WIN64)
  if(ioctlsocket((SOCKET)sockfd, FIONBIO, &on) != 0) {
    perror("ioctlsocket");
    goto error;
  }
#else
  if(fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0) {
    perror("fcntl");
    goto error;
  }
#endif

  if(bind(sockfd, res->ai_addr, res->ai_addrlen) < 0) {
    perror("bind");
    goto error;
  }
  freeaddrinfo(res);
  if(listen(sockfd, 1) < 0) {
    perror("listen");
    goto error;
  }

  FD_ZERO(&master);
  FD_ZERO(&read_fds);
  FD_SET(sockfd, &master);
  fdmax = sockfd;
  
  while(1) {
    read_fds = master;
    select(fdmax+1, &read_fds, NULL, NULL, NULL);

    for(i = 0; i < fdmax; i++) {
      if(FD_ISSET(i, &read_fds)) {
        if(i == sockfd) { /* get a new connection */
          clientfd = accept(sockfd, &client, &addrlen);
          if(clientfd < 0) {
	    perror("accept");
          } else {
	    FD_SET(clientfd, &read_fds);
	    if(clientfd > fdmax)
	      fdmax = clientfd;
          }
	} else { /* handle data from client */
          do {
	    memset(entry, 0, sizeof entry);
	    send(i, "Enter password: ", 16, 0);
            recv(i, entry, sizeof entry, 0);
          } while(strncmp(entry, "AW96B6", sizeof entry) != 0);
#if defined(_WIN32) || (_WIN64)
          closesocket((SOCKET)i);
#else
          close(i);
#endif
          break;
	}
      }
    }
  }
#if defined(_WIN32) || (_WIN64)
  closesocket((SOCKET)sockfd);
  WSACleanup();
#else
  close(sockfd);
#endif
  return 0;

 error:
#if defined(_WIN32) || (_WIN64)
  closesocket((SOCKET)sockfd);
  WSACleanup();
#else
  close(sockfd);
#endif
  return 1;
}
