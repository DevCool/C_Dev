#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* how big can the buffer grow */
#define MAXLINE 1

/* function prototypes */
char* getline(void);
char* strip_line(const char *s);

/* getline() - func to get user input (a line of text) with
 * out the use of fgets() function.
 */
char* getline(void) {
	char *buf, c;
	int i, j;

	j = MAXLINE*sizeof(char);
	buf = malloc(j);
	if(buf != NULL) {
		i = 0;
		while(1) {
			c = getchar();
			if(c == (~(1)+1) || c == 0x0A) {
				j += MAXLINE*sizeof(char)+1;
				buf = realloc(buf, j);
				if(buf == NULL)
					break;
				*(buf+i) = 0x0A;
				*(buf+i+1) = 0x00;
				return buf;
			} else if(c == 0x08) {
				*(buf+i) = 0x00;
				--i;
				if(i < j) {
					j -= MAXLINE*sizeof(char);
					buf = realloc(buf, j);
					if(buf == NULL)
						break;
				}
			} else {
				*(buf+i) = c;
				++i;
			}

			if(i >= j) {
				j += MAXLINE*sizeof(char);
				buf = realloc(buf, j);
				if(buf == NULL)
					break;
			}
		}
	}
	puts("Error: out of memory.");
	return NULL;	/* returns NULL char pointer */
}

/* strip_line() - strips last character off the given string.
 */
char* strip_line(const char *s) {
	char *ret;

	ret = malloc(strlen(s));
	if(ret != NULL) {
		memcpy(ret, s, strlen(s)-1);
		*(ret+strlen(s)) = 0x00;
		return ret;
	}
	puts("Error: Out of memory.");
	return NULL;	/* returns NULL char pointer */
}

/* test() - tests out how good my functions are.
 */
void test(void) {
	char *buf, *strip;

	printf("Enter your password: ");
	buf = getline();
	if(buf != NULL) {
		strip = strip_line(buf);
		if(strncmp(strip, "n385neJ", strlen(strip)+1) == 0)
			puts("You've entered the passcode correctly.");
		else
			puts("You were wrong!");
		free(strip);
		free(buf);
	}
	puts("End of test.");
}

