/**********************************************************************
 * sendmsg.c - Sends a message across the network to someone else.
 * by Philip Simonson
 **********************************************************************
 */

#if defined(_WIN32) || (_WIN64)
	#include <winsock2.h>
	#include <windows.h>
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define MAXLINE 79

int getln(char *buf, const char *prompt, int size);

int main(int argc, char *argv[]) {
	char buffer[MAXLINE];
	int i;

	printf("Enter text below, when you want to quit just press\nWindows/Linux Ctrl+Z/Ctrl+D on newline. Then\npress 'Return' key.\n");
	while((i = getln(buffer, "> ", MAXLINE)))
		if(i > 0 || i < MAXLINE)
			printf("What you typed is below this line.\n%s\n", buffer);
		else
			printf("You've entered way to many characters : %d\n", i);
	puts("Program has ended.");
	return 0;
}

int getln(char *buf, const char *prompt, int size) {
	int c, i, j;

	if(prompt != NULL)
		printf("%s", prompt);
	j = 0;
	for(i = 0; (c = getchar()) != (~1 + 1) && c != 0x0D; ++i)
		if(i < size-3) {
			buf[i] = c;
			++j;
		}
	if(c == 0x0D) {
		buf[i] = c;
		++i;
		++j;
	}
	buf[j] = 0x00;
	++j;
	return i;
}
