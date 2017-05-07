#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main(argc, argv)
	int argc;
	char **argv;
{
	int editor();
	int reader();
	char buffer[512][80];
	int res;

	if(argc < 2 && argc > 3) {
		fprintf(stderr, "Usage: %s [-e] <filename>\n", argv[0]);
		return 0;
	}

	if(argc == 2) {
		res = reader(argv[1],buffer);
	} else if(argc == 3 && argv[1][0] == '-' &&
		argv[1][1] == 'e') {
		res = editor(argv[2]);
	}
	return(res);
}

int editor(filename)
	const char *filename;
{
	char buf[BUFSIZ];
	int pos;
	int c;
	FILE *file;

	if((file = fopen(filename,"w+")) == NULL) {
		printf("Cannot open file for writing.\n");
		return -1;
	}
	do {
		pos = 0;
		memset(buf, 0, sizeof(buf));
		while(1) {
			c = getchar();
			if(c == EOF || c == 0x0D) {
				if(pos<BUFSIZ-1) {
					buf[pos] = 0x0A;
					buf[pos+1] = 0x00;
				}
				if(pos<BUFSIZ) {
					buf[pos] = 0x00;
				}
				break;
			} else if(c == 0x08) {
				buf[--pos] = 0x00;
			} else {
				buf[pos] = c;
			}
			++pos;
		}
		fprintf(file, "%s", buf);
	} while(c != EOF);
	fclose(file);
	printf("File written successfully.\n");
}

int reader(filename, buf)
	const char *filename;
	char buf[512][80];
{
	FILE *file;
	int cols;
	int rows;
	int c;

	memset(buf, 0, 512*80);
	if((file = fopen(filename,"r")) == NULL) {
		printf("Cannot open input file for reading.\n");
		return 1;
	}
	cols = 0;
	rows = 0;
	errno = 0;
	while((c = fgetc(file)) != EOF) {
		if(c == 0x0A || c == 0x0D) {
			++rows;
		}
		buf[rows][cols] = c;
		++cols;
	}
	if(errno != 0) {
		printf("Error: reading file.\n");
		fclose(file);
		return -1;
	}
	fclose(file);
	for(rows=0; rows<=512; ++rows) {
		for(cols=0; cols<80; ++cols) {
			putchar(buf[rows][cols]);
		}
	}
	return 0;
}

