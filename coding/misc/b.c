#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ASSERT(T, M) if((T)) { fprintf(stderr, "[ABORTED] : %s\n[LINE:%d] [FILE:%s]\n", M,\
	__LINE__, __FILE__); exit(-1); }

int
str_len(s)
const char *s;
{
	int cnt = 0;
	while(*s++ != 0) cnt++;
	return cnt;
}

void
mem_cpy(s, s2, a)
char *s;
const char *s2;
char a;
{
	ASSERT(strlen(s)+1 >= str_len(s2)+1, "First argument not >= size of second argument!");
	for(; *s2 && s2!=(s2+str_len(s2)); s++,s2++)
		if(isalpha(*s2) && a == 1)
			*s = tolower(*s2);
		else if(isalpha(*s2) && a == 2)
			*s = toupper(*s2);
		else
			*s = *s2;
	*s = 0;
}

char*
to_lower(s)
char *s;
{
	char s2[str_len(s)+1], *tmp = NULL;
	memset(s2,0,sizeof(s2));
	mem_cpy(s2, s, 1);
	tmp = &s2[0];
	return tmp;
}

char*
to_upper(s)
char *s;
{
	char s2[str_len(s)+1], *tmp = NULL;
	memset(s2,0,sizeof(s2));
	mem_cpy(s2, s, 2);
	tmp = &s2[0];
	return tmp;
}

#define MESSAGE "HellO WoRLd.\0"

int
main()
{
	char mem1[str_len(MESSAGE)+1];
	char mem2[str_len(MESSAGE)+1]; 
	char mem3[str_len(MESSAGE)+1];
	char *mem4 = &mem3[0];

	mem_cpy(mem1, to_lower(MESSAGE), 0);
	mem_cpy(mem2, to_upper(MESSAGE), 0);
	mem_cpy(mem3, MESSAGE, 0);
	printf("mem1 : %s [%d]\n", mem1, str_len(mem1));
	printf("mem2 : %s [%d]\n", mem2, str_len(mem2));
	printf("mem3 : %s [%d]\n", mem3, str_len(mem3));
	while(*mem4!=0 && (mem3!=(mem4+sizeof(mem4)))) {
		putchar(*mem4);
		++mem4;
	}
	putchar(0x0d);
	return 0;
}
