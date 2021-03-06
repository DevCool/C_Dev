/* #define WINDOWS */
#include "sock_help.h"

#define MAXCLIENTS 10

int main()
{
#ifdef WINDOWS
	FDSET readfds;
#else
	fd_set readfds;
#endif
	extern int process_commands();
	struct sockaddr_in address;
	int sock, newfd;
	char buf[512];
	char msg[256];
	int clients[MAXCLIENTS];
	int maxfd, addrlen;
	int bytes, i;
	int done;

	for (i = 0; i < MAXCLIENTS; i++)
		clients[i] = 0;

	sock = create_server(0, 5555, "0.0.0.0");
	if (sock < 0)
		return -1;
	done = 0;
	while (!done) {
		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		maxfd = sock;

		for (i = 0; i < MAXCLIENTS; i++) {
			newfd = clients[i];

			if (newfd > 0)
				FD_SET(newfd, &readfds);
			if (newfd > maxfd)
				maxfd = newfd;
		}

		if (select(maxfd+1, &readfds, NULL, NULL, NULL) < 0) {
			perror("select");
			exit(4);
		}

		if (FD_ISSET(sock, &readfds)) {
			newfd = accept(sock, (struct sockaddr*)&address, (socklen_t*)&addrlen);
			if (newfd < 0) {
				perror("accept");
				done = 1;
			}

			getpeername(newfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
			printf("Client %s connected on port %d\n",
				inet_ntoa(address.sin_addr),
				ntohs(address.sin_port));

			memset(msg, 0, sizeof(msg));
			sprintf(msg, "Type something: ");
			bytes = send(newfd, msg, strlen(msg), 0);

			for (i = 0; i < MAXCLIENTS; i++) {
				if (clients[i] == 0) {
					clients[i] = newfd;
					break;	
				}
			}
		}

		for (i = 0; i < MAXCLIENTS; i++) {
			newfd = clients[i];
			if (FD_ISSET(newfd, &readfds)) {
				getpeername(newfd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
				bytes = getln_remote(newfd, buf, sizeof(buf));
				if (bytes <= 0) {
					if (bytes == 0)
						puts("Connection closed.");
					else
						perror("recv");
					close_conn(clients[i]);
					clients[i] = 0;
				} else {
					strip_cmd(buf, &bytes);
					process_commands(&clients[i], buf); 
				}
			}
		}
	}
	for (i = 0; i < MAXCLIENTS; i++) {
		close_conn(clients[i]);
		clients[i] = 0;
	}
	close_conn(sock);
	return 0;
}

int
process_commands(sock, cmd)
int *sock;
const char *cmd;
{
	struct sockaddr_in address;
	char msg[256];
	int addrlen;

#ifdef WINDOWS
	getpeername(*sock, (struct sockaddr*)&address, (int*)&addrlen);
#else
	getpeername(*sock, (struct sockaddr*)&address, (socklen_t*)&addrlen);
#endif

	if (pstricmp(cmd, "exit") == 0) {
		printf("Client %s disconnected.\n",
			inet_ntoa(address.sin_addr));
		close_conn(*sock);
		*sock = 0;
	} else if (pstricmp(cmd, "help") == 0) {
		memset(msg, 0, sizeof msg);
		sprintf(msg, "Commands: [help, exit]\r\n");
		if (send(*sock, msg, strlen(msg), 0) != strlen(msg))
			return 1;
	} else {
		printf("Recv from client: %s\n", cmd);
	}
	memset(msg, 0, sizeof(msg));
	sprintf(msg, "Type something: ");
	if (send(*sock, msg, strlen(msg), 0) != strlen(msg))
		return 1;
	return 0;
}

