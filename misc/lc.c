#include <stdio.h>

/* Program for counting lines */

int main(int argc, char **argv)
{
	int res = 1;
	int c;
	FILE *fp;

	if(argc != 2 && argc > 2) {
		puts("Warning: You need a file.\n");
		return -2;
	}

	fp = fopen(argv[1], "rt");
	if(!fp) {
		puts("Error: File doesn't exist.\n");
		return -1;
	}

	while(!feof(fp)) {
		c = fgetc(fp);
		if(c == '\n') ++res;
	}
	fclose(fp);
	printf("Lines in file: %d\n", res);
	return 0;
}
