#include <stdio.h>
#include <stdlib.h>

extern char* getline(void);
extern char* strip_line(const char *s);

int main(void) {
	char *buf, *strip;

	printf("Enter a line of text below...\n");
	buf = getline();
	if(buf != NULL) {
		printf("Your line of text is..\n%s\n", buf);
		printf("Your line of text stripped is..\n%s\n", (strip = strip_line(buf)));
	}
	printf("End of program!\n");
	free(strip);
	free(buf);
	return 0;
}
