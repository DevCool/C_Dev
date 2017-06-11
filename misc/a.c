#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **args, char **envp)
{
	char buf[512], *tmp;

	memset(buf, 0, sizeof(buf)/sizeof(char));

	tmp = getenv("QUERY_STRING");
	memcpy(buf, tmp, sizeof(buf)/sizeof(char));
	*(buf+512) = 0;

	printf("<Content-type:text/plain>\n\n");
	printf("QUERY_STRING : %s\n", buf);

	return 0;
}