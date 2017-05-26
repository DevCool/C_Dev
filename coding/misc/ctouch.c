#include <stdio.h>
#include <stddef.h>
#include <string.h>

#define MAXBUF 1024

int getln();

int main(argc,argv)
	int argc;
	char **argv;
{
	char s[MAXBUF];

	while(getln(s,sizeof(s)) > 0)
		printf("Text entered: %s\n",s);
	return 0;	
}

int getln(str,size)
	char str[];
	size_t size;
{
	int c,i,j;

	if(str == NULL)
		return -1;
	j = 0;
	memset(str,0,size);
	for(i = 0; (c = getchar()) != EOF && c != 0x0D; ++i)
		if(i < size-1) {
			str[i] = c;
			++j;
		}
	if(c == 0x0D) {
		str[i] = c;
		++i;
		++j;
	}
	++j;
	str[j] = 0;
	return i;
}
