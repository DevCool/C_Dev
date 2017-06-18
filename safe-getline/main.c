#include <stdio.h>
#include <stdlib.h>

extern char* getline1(void);
extern char* strip_line(const char *s);
extern void test(void);
extern void test2(void);
extern void test3(void);

/* main() - entry point for my program.
 */
int main(void) {
	char *buf, *strip;

	test();
	printf("Enter a line of text below...\n");
	buf = getline1();
	if(buf != NULL) {
		printf("Your line of text is..\n%s\n", buf);
		printf("Your line of text stripped is..\n%s\n", (strip = strip_line(buf)));
		free(strip);
		free(buf);
	}
	/* my other getline function - getline2() */
	test2();
	/* testing my singly linked list */
	test3();
	/* let user know end of program reached, return success */
	printf("End of program!\n");
	return 0;
}

