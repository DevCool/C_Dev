#include <stdio.h>

int
str_len(s)
const char *s;
{
	int retval;
	for(retval=0; *s++!=0; ++retval);
	return retval;
}

int
main()
{
	char *message = "Hello world!\0";
	const char *p = &message[str_len(message)+1];
	while(1)
		if(message!=p) putchar(*message++);
		else {
			putchar((char)0x0d);
			break;
		}
	return 0;
}
