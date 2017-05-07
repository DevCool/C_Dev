#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(argc, argv)
	int argc;
	char **argv;
{
	int editor();

	if(argc != 2) {
		fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
		return 0;
	}

	return(editor(argv[1]));
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

