#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 2

#define assert(A, T) ({ if(!(T)) fprintf(stderr,\
"Assert failed: [%s] aborted.\n[LINE:%d] [FILE:%s]\n", A, __LINE__, __FILE__);\
if(A != NULL) free(A); -1; })

#define cnt_buf(x)\
({\
	char* p = (x);\
	int cnt = 0;\
	while(*p++ != 0)\
		++cnt;\
	cnt;\
})

char*
getln(a)
char a;
{
	int c = 0;
	int size = BUFSIZE;
	char *buf = NULL;
	int pos = 0;

	buf = realloc(buf, size);
	if(buf == NULL) return NULL;

	while (1) {
		c = getchar();
		if (c == -1 || c == 10) {
			if ((pos+2) >= size && a) {
				size += BUFSIZE;
				buf = realloc(buf, size);
				if (buf == NULL) {
					printf("Out of memory.\n");
					break;
				}
				*(buf + pos) = 0x0D;
				*(buf + pos + 1) = 0x00;
			} else {
				*(buf + pos) = 0x00;
			}
			return buf;
		}
		else {
			*(buf + pos) = c;
		}
		++pos;

		if (pos >= size) {
			size += BUFSIZE;
			buf = realloc(buf, size);
			if (buf == NULL) {
				printf("Out of memory.\n");
				break;
			}
		}
	}

	return NULL;
}

int
main()
{
	char* test = "This is a test string.";
	char* buf = NULL;

	printf("%s\nBuffer Count: %d\n", test, cnt_buf(test));
	buf = getln(1);
	printf("You typed:\n%s\nBuffer Count: [%d]\n", buf, cnt_buf(buf));
	if (buf != NULL) free(buf);

	buf = getln(0);
	printf("You typed:\n%s\nBuffer Count: [%d]\n", buf, cnt_buf(buf));
	if (buf != NULL) free(buf);

	return 0;
}
