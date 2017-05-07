#include <stdio.h>
#include <string.h>

int main(argc,argv)
	int argc;
	char **argv;
{
	int editor();
	char buf[1024*80+24];

	if(argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return 1;
	}

	return(editor(argv[1],buf));
}

int editor(filename,buffer)
	const char *filename;
	char *buffer;
{
	FILE *file;
	char buf[1024*80+24];
	int lines = 1024;
	int x, y;
	int c;


	memset(buf,0,sizeof(buf));
	if((file = fopen(filename,"r+")) == NULL) {
		printf("Error: cannot open file for reading/writing.\n");
		return 1;
	}
	x = 0;
	y = 0;
	while((c = fgetc(file)) != EOF) {
		if(c != EOF) {
			putchar(c);
			buf[lines*x+y] = c;
		}
	}
	fclose(file);
	buffer = buf;
	return 0;
}

