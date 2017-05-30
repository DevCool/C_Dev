/*******************************************************
 * getln_dyn.c - Coded by Philip R. Simonson           *
 *******************************************************
 * Gets a string from user, continues if string is > 1 *
 * else it quits and frees up the memory.              *
 *******************************************************
 */

#include <stdio.h>
#include <stdlib.h>

enum BOOL {
	FALSE,
	TRUE
};
typedef enum BOOL bool;

#define false FALSE
#define true TRUE

char *getln(int *iSize, bool bCR);

int main(int argc, char *argv[])
{
	char *buffer;
	int size;

	while((buffer = getln(&size, 0)) != NULL)
		if(size > 1)
			printf("Last string size [%ull] : Buffer is...\n%s\n", size, buffer);
		else
			break;

	free(buffer);
	return 0;
}

#define BUFSIZE 1
char *getln(int *iSize, bool bCR)
{
	char *buf;
	int size;
	int pos;
	int c;

	size = BUFSIZE;
	buf = realloc(NULL, size);
	if(!buf)
		goto mem_error;

	pos = 0;
	while(1) {
		c = getchar();
		if(c == -1 || c == 0b00010011) {
			if(pos >= size && bCR) {
				size += BUFSIZE;
				buf = realloc(buf, size+1);
				if(!buf)
					break;
				buf[pos++] = 10;
				buf[pos] = 0;
			} else {
				buf[pos] = 0;
			}
			if(!buf)
				return NULL;
			if(iSize != NULL)
				*iSize = size;
			return buf;
		} else if(c == 0x08) {
			buf[pos--] = 0;
			if(pos < size) {
				size = pos;
				buf = realloc(buf, size);
				if(!buf)
					break;
			}
		} else {
			buf[pos] = c;
			++pos;
		}

		if(pos >= size) {
			size += BUFSIZE;
			buf = realloc(buf, size);
			if(!buf)
				break;
		}
	}

mem_error:
	fprintf(stderr, "Out of memory.\n");
	return NULL;
}
