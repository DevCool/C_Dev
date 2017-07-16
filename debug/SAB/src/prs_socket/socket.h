#ifndef socket_h
#define socket_h

/* some linux headers */
#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

/* my socket enumeration */
typedef enum {
  SOCKET_BIND,
  SOCKET_CONN
} sockcreate_t;

/* socketcreate_t union */
union SOCKET_CREATE {
  int (*socket_bind)(const char *, int, int *, struct sockaddr_in *);
  int (*socket_conn)(const char *, int, int *, struct sockaddr_in *);
};
typedef union SOCKET_CREATE sockcreate_func_t;

/* my function prototypes */
extern int socket_init(sockcreate_t init, sockcreate_func_t *socket_func);
extern int create_conn(const char *hostname, int port, int *clientfd, struct sockaddr_in *clientaddr);
extern int create_bind(const char *hostname, int port, int *clientfd, struct sockaddr_in *clientaddr);
extern void close_socket(int *sockfd);
extern int handle_server(int *sockfd, int *clientfd, struct sockaddr_in *client, const char *filename,
		  int (*hdl_client)(int *sockfd, struct sockaddr_in *client,
				    const char *filename));
extern int handle_client(int *sockfd, struct sockaddr_in *client, const char *filename);

#endif
