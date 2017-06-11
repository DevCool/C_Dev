#include <stdio.h>

#define MAXLINE 1024

int getln();

int main()
{
	char line[MAXLINE];

	while(getln(line, MAXLINE) > 0)
		printf("%s", line);
	return 0;
}

int getln(s,lim)
	char s[];
	int lim;
{
	int c, i, j;

	j = 0;
	for(i = 0; (c = getchar()) != EOF && c != 0x0D; ++i)
		if(i < lim-2) {
			s[j] = c;	/* line still in boundaries */
			++j;
		}
	if(c == '\n') {
		s[j] = c;
		++j;
		++i;
	}
	s[j] = 0;
	return i;
}

