#include <stdio.h>
#include <string.h>

enum BOOL {
	FALSE,
	TRUE
};
typedef enum BOOL bool;

#define false FALSE
#define true TRUE

typedef unsigned long long int size_t;

int getln(char *s, size_t size, int ch);

int main(int argc, char **argv)
{
	char buf[64];
	bool bRun = true;

	/* Some main loop */
	while(bRun) {
		printf("Enter password: ");
		getln(buf, sizeof(buf), '\n');

		if(strncmp(buf, "passcode\n", sizeof(buf)) == 0)
			break;
		else
			printf("Wrong password.\n");
	}

	return 0;
}

int getln(char *s, size_t size, int ch)
{
	int c, i, j;

	j = 0;
	for(i = 0; (c = getchar()) != EOF && c != ch; ++i)
		if(i < size-2) {
			s[i] = c;
			++j;
		}
	if(c == ch) {
		s[i] = c;
		++i;
		++j;
	}
	s[i] = 0;
	++i;
	return j;
}
