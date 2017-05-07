#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(argc, argv)
	int argc;
	char **argv;
{
	int editor();
	int reader();
	char *buffer;
	int res;

	if(argc < 2 && argc > 3) {
		fprintf(stderr, "Usage: %s [-e] <filename>\n", argv[0]);
		return 0;
	}

	if(argc == 2) {
		res = reader(argv[1],buffer);
		free(buffer);
		if(res != 0) return res;
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
	char *buf;
{
	FILE *file;
	int pos;
	int c;

	if((file = fopen(filename,"r")) == NULL) {
		printf("Cannot open input file for reading.\n");
		return 1;
	}
	if((buf = calloc(1, ftell(file))) == NULL) {
		printf("Cannot allocate memory for size of file.\n");
		fclose(file);
		return -1;
	}
	rewind(file);
	pos = 0;
	while((c = fgetc(file)) != EOF) {
		if(fputc(c,stdout) == EOF) {
			printf("Unable to read file.\n");
			fclose(file);
			return 1;
		}
		*(buf+pos) = c;
		++pos;
	}
	fclose(file);
	return 0;
}

