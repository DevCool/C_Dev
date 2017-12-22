/* #define WINDOWS */
#include "sock_help.h"

int main()
{
#ifdef WINDOWS
	FDSET readfds;
#else
	fd_set readfds;
#endif
	int sock;
	char buf[256];
	int bytes;
	int len;

	sock = create_client(0, 5555, "127.0.0.1");
	if (sock < 0)
		return -1;
	while (1) {
		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		FD_SET(STDIN_FILENO, &readfds);
		if (select(sock+1, &readfds, NULL, NULL, NULL) < 0) {
			perror("select");
			close_conn(sock);
			exit(1);
		}

		if (FD_ISSET(STDIN_FILENO, &readfds)) {
			len = getln(buf, sizeof(buf));
			bytes = send(sock, buf, len, 0);
			if (bytes <= 0) {
				if (bytes == 0)
					puts("Connection closed.");
				else
					perror("send");
				break;
			}
		}
		if (FD_ISSET(sock, &readfds)) {
			bytes = recv(sock, buf, sizeof(buf), 0);
			buf[bytes] = '\0';
			if (bytes <= 0) {
				if (bytes == 0)
					puts("Connection closed.");
				else
					perror("recv");
				break;
			}
			printf("%s", buf);
			fflush(stdout);
		}
	}
	close_conn(sock);
	return 0;
}
