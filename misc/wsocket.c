#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>

int main()
{
	WSADATA data;
	int err;

	WSAStartup(MAKEWORD(2,2), &data);
	if(err != 0) {
		puts("Couldn't start winsock2!");
		return -1;
	}

	if (LOBYTE(data.wVersion) != 2 || HIBYTE(data.wVersion) != 2) {
		printf("Couldn't find a usable version of Winsock.dll\n");
		return -1;
	}
#if !defined(NDEBUG)
	else
		printf("Usable Winsock.dll found!\n");
#endif

	/* Do stuff here */

	/* cleanup */
	WSACleanup();

	return 0;
}