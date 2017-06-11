/* Touch command for windows.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(argc, argv)
	int argc;
	char **argv;
{
	char tmp[256];
	int i;

	for(i=1; i<argc; ++i) {
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp,"copy NUL %s",argv[i]);
		system(tmp);
	}
	return(0);
}
