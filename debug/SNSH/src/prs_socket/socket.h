#ifndef socket_h
#define socket_h

/* some linux headers */
#if defined(_WIN32) || (_WIN64)
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>
#define DLL_EXPORT __declspec(dllexport)
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#if defined(_WIN32) || (_WIN64)
/* my socket enumeration */
DLL_EXPORT enum SOCKCREATE {
  SOCKET_BIND,
  SOCKET_CONN
};
typedef DLL_EXPORT enum SOCKCREATE sockcreate_t;

/* socketcreate_t union */
DLL_EXPORT union SOCKET_CREATE {
  int (*socket_bind)(const char *, int, int *, struct sockaddr_in *);
  int (*socket_conn)(const char *, int, int *, struct sockaddr_in *);
};
typedef DLL_EXPORT union SOCKET_CREATE sockcreate_func_t;

/* my function prototypes */
int DLL_EXPORT socket_init(sockcreate_t init, sockcreate_func_t *socket_func);
int DLL_EXPORT create_conn(const char *hostname, int port, int *clientfd, struct sockaddr_in *clientaddr);
int DLL_EXPORT create_bind(const char *hostname, int port, int *clientfd, struct sockaddr_in *clientaddr);
void DLL_EXPORT close_socket(int *sockfd);
int DLL_EXPORT handle_server(int *sockfd, int *clientfd, struct sockaddr_in *client, const char *filename,
		  int (*hdl_client)(int *sockfd, struct sockaddr_in *client,
				    const char *filename));
int DLL_EXPORT handle_client(int *sockfd, struct sockaddr_in *client, const char *filename);
#else
/* sockcreate_t enum */
enum SOCKCREATE {
  SOCKET_BIND,
  SOCKET_CONN
};
typedef enum SOCKCREATE sockcreate_t;

/* socketcreate_t union */
union SOCKET_CREATE {
  int (*socket_bind)(const char *, int, int *, struct sockaddr_in *);
  int (*socket_conn)(const char *, int, int *, struct sockaddr_in *);
};
typedef union SOCKET_CREATE sockcreate_func_t;

int socket_init(sockcreate_t init, sockcreate_func_t *socket_func);
int create_conn(const char *hostname, int port, int *clientfd, struct sockaddr_in *clientaddr);
int create_bind(const char *hostname, int port, int *clientfd, struct sockaddr_in *clientaddr);
void close_socket(int *sockfd);
int handle_server(int *sockfd, int *clientfd, struct sockaddr_in *client, const char *filename,
		  int (*hdl_client)(int *sockfd, struct sockaddr_in *client,
				    const char *filename));
int handle_client(int *sockfd, struct sockaddr_in *client, const char *filename);
#endif

#endif
