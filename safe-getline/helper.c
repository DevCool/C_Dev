#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* define size_t */
typedef unsigned long int size_t;

/* how big can the buffer grow */
#define MAXLINE 1
#define LINESIZE 80

/* function prototypes */
char* getline(void);
int getline2(char* s, int size);
void pzero(char* s, size_t size);
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
		pzero(buf, j);
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

/* getline2() - gets a line of text with allocating memory for it. Plus
 * it removes a character when you backspace.
 */
int getline2(char* s, int size) {
	int c, i, j;

	j = 0;
	for(i = 0; (c = getchar()) != EOF && c != 0x0A; )
		if(i < size-2) {
			if(c == 0x08) {
				*(s+i) = 0;
				if(i > 0)
					--i;
				continue;
			}
			*(s+i) = c;
			++i;
			++j;
		}
	if(c == 0x0A) {
		*(s+i) = 0x0A;
		++i;
		++j;
	}
	*(s+j) = 0;
	++j;
	return i;
}

/* pzero() - zero block of memory that s is pointing at.
 */
void pzero(char* s, size_t size) {
	size_t i;

	for(i = 0; i < size; i++)
		*(s+i) = 0x00;
}

/* strip_line() - strips last character off the given string.
 */
char* strip_line(const char* s) {
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

	printf(" *** Test 1 ***\n\n");
	buf = strip = NULL;
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

/* test2() - tests out how good my getline2 function is.
 */
void test2(void) {
	char s[BUFSIZ], *tmp;

	tmp = NULL;
	printf("\n *** Test 2 ***\n\n");
	do {
		if(tmp != NULL)
			free(tmp);
		printf("LOGON: ");
		pzero(s, sizeof(s));
		getline2(s, sizeof(s));
		tmp = strip_line(s);
		if(strncmp(tmp, "]exit", strlen(tmp)+1) == 0)
			break;
		else if(strncmp(tmp, "aw96b6", strlen(tmp)+1) == 0)
			puts("Access granted!");
		else
			puts("Access denied!");
	} while(1);
	printf("\nEnd of test 2.\n");
}

