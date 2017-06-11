/* Touch command for any system
 */

#include <stdio.h>

int main(argc,argv)
	int argc;
	char **argv;
{
	FILE *file;
	int i,j;

	if(argc < 2) {
		printf("Usage: %s <filename> <...>\n"
			"Enter file names with path.\n",argv[0]);
		return 0;
	}
	j = 0;
	for(i = 1; i < argc; ++i) {
		file = fopen(argv[i],"w");
		if(file == NULL) {
			printf("Could not create file: %s\n",argv[i]);
		}
		fclose(file);
		++j;
	}
	printf("File(s) created : %d.\n",j);
	return 0;
}
