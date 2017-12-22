/*****************************************************
 * sock_help.h - Networking Library (Helper Library)
 * by Philip "5n4k3" Simonson
 *****************************************************
 */

#define SOCK_HELP

#ifdef WINDOWS
#include <winsock2.h>
#include <windows.h>
#include <io.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#define MAX_LINE	16384
#define MAX_CLIENTS	10

/* set_nonblocking: set a socket to non-blocking IO */
static int
set_nonblocking(sock)
#ifdef WINDOWS
SOCKET sock;
#else
int sock;
#endif
{
#ifdef WINDOWS
	unsigned long mode = 1;
	return ioctlsocket(sock, FIONBIO, &mode);
#else
	return fcntl(sock, F_SETFL, O_NONBLOCK);
#endif
}

/* create_server:  creates a server; return socket fd */
static int
create_server(nonblocking, port, address)
int nonblocking;
int port;
const char *address;
{
	struct sockaddr_in server;
#ifdef WINDOWS
	SOCKET sock;
	WSADATA wsaData;
#else
	int sock;
#endif

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(address);

#ifdef WINDOWS
	if (WSAStartup(0x0202, &wsaData) != 0) {
		puts("Error: Could not start winsock.");
		return -1;
	}
	if ((sock = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, 0)) == INVALID_SOCKET) {
		puts("Error: Could not create socket.");
		return -1;
	}
#else
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket");
		return -1;
	}
#endif
	if (nonblocking)
		set_nonblocking(sock);

	if (bind(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
		perror("bind");
		return -1;
	}
	printf("%s bound to port %d.\nWaiting for incoming connections...\n",
			inet_ntoa(server.sin_addr), port);

	if (listen(sock, MAX_CLIENTS) < 0) {
		perror("listen");
		return -1;
	}
	return sock;
}

/* create_client:  creates a client socket to connect to a server */
static int
create_client(nonblocking, port, address)
int nonblocking;
int port;
const char *address;
{
	struct sockaddr_in server;
#ifdef WINDOWS
	SOCKET sock;
	WSADATA wsaData;

	if (WSAStartup(0x0202, &wsaData) != 0) {
		fprintf(stderr, "Error: Cannot start winsock library.\n");
		return -1;
	}
#else
	int sock;
#endif

	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = inet_addr(address);

#ifdef WINDOWS
	if ((sock = WSASocket(AF_INET, SOCK_STREAM,
		IPPROTO_TCP, NULL, 0, 0)) == INVALID_SOCKET) {
		fprintf(stderr, "Error: Cannot create the client socket.\n");
		return -1;
	}
#else
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("socket");
		return -1;
	}
#endif
	if (nonblocking)
		set_nonblocking(sock);

	if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
		fprintf(stderr, "Error: Cannot connect to %s.\n",
				inet_ntoa(server.sin_addr));
		return -1;
	}
	return sock;
}

/* send_msg:  send message to remote socket */
static int
send_msg(sock, msg)
#ifdef WINDOWS
SOCKET sock;
#else
int sock;
#endif
const char *msg;
{
	int bytes;
	if ((bytes = send(sock, msg, strlen(msg), 0)) != strlen(msg)) {
		puts("Error: Sending message failed.");
		return bytes;
	}
	return bytes;
}

/* close_conn:  closes remote connection */
static void
close_conn(sock)
#ifdef WINDOWS
SOCKET sock;
#else
int sock;
#endif
{
#ifdef WINDOWS
	closesocket(sock);
	WSACleanup();
#else
	close(sock);
#endif
}

/* getln_remote:  gets a remote string */
static int
getln_remote(sock, s, size)
int sock;
char s[];
size_t size;
{
	size_t i;
	size_t j;
	char c;

	j = 0;
	memset(s, 0, sizeof(size));
	for(i = 0; i < size-1 && recv(sock, &c, 1, 0) && c != '\n' && c != '`';) {
		if(c == 0x08) {
			--i;
			s[i] = 0;
			send(sock, " \b", 2, 0);
			continue;
		}
		s[i++] = c;
		++j;
	}
	if(c == '\n') {
		s[i++] = c;
		++j;
	}
	s[i] = 0;
	return j;
}


/* strip_cmd:  strips off newlines */
static void
strip_cmd(buf, len)
char buf[];
int *len;
{
	int i = *len;

	while (i >= 0) {
		if (isprint(buf[i])) {
			buf[++i] = '\0';
			break;
		}
		--i;
	}
	*len = i;
}

/* get_cmd:  gets a remote string; removing newlines */
static void
get_cmd(sock, buf, size)
int sock;
char buf[];
int size;
{
	int i;

	memset(buf, 0, size);
	i = getln_remote(sock, buf, size);
	strip_cmd(buf, &i);
}

/* getln:  get input string from keyboard */
static int
getln(s, lim)
char s[];
int lim;
{
	int c, i, j;

	j = 0;
	for (i = 0; i < lim-2 && (c = getchar()) != EOF && c != '\n'; ) {
		if (c == '\b') {
			s[--i] = '\0';
			--j;
			continue;
		}
		s[i++] = c;
		++j;
	}
	if (c == '\n') {
		s[i++] = c;
		++j;
	}
	s[i] = '\0';
	return j;
}

/* pstrcmp() - compare string s1 with s2 */
static int
pstrcmp(p1, p2)
const char *p1;
const char *p2;
{
	register const unsigned char *s1 = (const unsigned char *)p1;
	register const unsigned char *s2 = (const unsigned char *)p2;
	unsigned char c1, c2;

	do {
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
			return c1-c2;
	} while (c1 == c2);

	return c1-c2;
}

/* pstricmp() - *nix doesn't have stricmp like windows */
static int
pstricmp(p1, p2)
const char *p1;
const char *p2;
{
	register const unsigned char *s1 = (const unsigned char *)p1;
	register const unsigned char *s2 = (const unsigned char *)p2;
	unsigned char c1, c2;

	do {
		c1 = (unsigned char)*s1++;
		c2 = (unsigned char)*s2++;
		if (c1 == '\0')
			return tolower(c1)-tolower(c2);
	} while (tolower(c1) == tolower(c2));

	return tolower(c1)-tolower(c2);
}

