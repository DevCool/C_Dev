#include <stdio.h>
#include <errno.h>

int main(argc, argv)
	int argc;
	char **argv;
{
	void dosform();
	int i;

	for(i=1; i<argc; ++i) {
		dosform(argv[i]);
	}
	return(0);
}

void dosform(filename)
	const char *filename;
{
	char tempname[256];
	FILE *file;
	FILE *tfile;
	int c;

	sprintf(tempname,"/tmp/%s",filename);
	if((file = fopen(filename,"r")) == NULL) {
		printf("  unable to open original file for input\n");
		return;
	}
	if((tfile = fopen(tempname,"w")) == NULL) {
		printf("  unable to open temporary file for output\n");
		fclose(file);
		return;
	}
	errno = 0;
	while((c = fgetc(file)) != EOF) {
		if(c == 0x0A) {
			if(fputc(0x0D,tfile) == EOF) {
				printf("  unable to write to temporary file\n");
				fclose(file);
				fclose(tfile);
				return;
			}
		}
		if(fputc(c,tfile) == EOF) {
			printf("  unable to write to temporary file\n");
			fclose(file);
			fclose(tfile);
			return;
		}
		errno = 0;
	}
	if(errno != 0) {
		printf("  error reading from original file\n");
		fclose(file);
		fclose(tfile);
		return;
	}
	fclose(file);
	fclose(tfile);

	if((tfile = fopen(tempname,"r")) == NULL) {
		printf("  unable to open temporary file for input\n");
		return;
	}
	if((file = fopen(filename,"w")) == NULL) {
		printf("  unable to open new file for output\n");
		fclose(tfile);
		return;
	}
	errno = 0;
	while((c = fgetc(tfile)) != EOF) {
		if(fputc(c, file) == EOF) {
			printf("  unable to write to new file\n");
			fclose(file);
			fclose(tfile);
			return;
		}
		errno = 0;
	}
	if(errno != 0) {
		printf("  error reading from temporary file\n");
		fclose(file);
		fclose(tfile);
		return;
	}
	fclose(file);
	fclose(tfile);
	printf("File written successfully\n");
}