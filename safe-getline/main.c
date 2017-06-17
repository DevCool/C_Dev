#include <stdio.h>
#include <stdlib.h>

extern char* getline(void);
extern char* strip_line(const char *s);
extern void test(void);

/* main() - entry point for my program.
 */
int main(void) {
	char *buf, *strip;

	test();
	printf("Enter a line of text below...\n");
	buf = getline();
	if(buf != NULL) {
		printf("Your line of text is..\n%s\n", buf);
		printf("Your line of text stripped is..\n%s\n", (strip = strip_line(buf)));
		free(strip);
		free(buf);
	}
	printf("End of program!\n");
	return 0;
}

