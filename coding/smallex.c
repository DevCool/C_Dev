#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define MAX_CMD 64
#define BUFSIZE 2


/********************************************************************************************************
 *                          Standard C Does NOT Include BOOL Variables unless
 ********************************************************************************************************
 * You make it yourself, or #include <stdbool.h> (BOOL was added as a keyword in C++ [which is C Extended]
 ********************************************************************************************************
 */

typedef enum BOOL {
	FALSE,
	TRUE
} bool;

#define false 0
#define true 1

char *getln(bool bNwLn)
{
	size_t size = BUFSIZE;
	char* buf = calloc(1, size);
	int p = 0;
	int c;

	while(1) {
		c = getchar();
		if(c == EOF || c == '\n' && bNwLn) {
			if (p+BUFSIZE >= size) {
				size += BUFSIZE;
				buf = realloc(buf, size);
				if(!buf) {
					printf("Out of memory!");
					break;
				}
			}
			*(buf+p) = '\n';
			*(buf+p+1) = '\0';
			return buf;
		}
		else if(c == EOF || c == '\n' && !bNwLn) {
			*(buf+p) = '\0';
			return buf;
		}
		else
			*(buf+p) = c;
		++p;

		if(p >= size) {
			size += BUFSIZE;
			buf = realloc(buf, size);
			if(!buf) {
				printf("Out of memory!\n");
				break;
			}
		}
	}

	return NULL;
}

int main()
{
	/* simple test */
	char *pInput;
	char cmd[MAX_CMD];

	printf("Enter input: ");
	pInput = getln(false);

	do {
		sscanf(pInput, "%s ", cmd);
		if(strncmp(cmd, "exit", MAX_CMD) == 0)
			break;
		if(strncmp(cmd, "hello", MAX_CMD) == 0)
			puts("Hello, master.");

		free(pInput);
		printf("Enter Input: ");
		pInput = getln(true);
	} while(true);
	printf("Input was:\n%s", pInput);
	if(pInput != NULL) free(pInput);
	return 0;
}