#include <stdio.h>
#include <errno.h>

int main(argc, argv)
	int argc;
	char **argv;
{
	void unixform();
	int i;

	for(i=1; i<argc; ++i) {
		unixform(argv[i]);
	}
}

void unixform(filename)
	char *filename;
{
	char tempname[256];
	FILE *file;
	FILE *tfile;
	int c;

	sprintf(tempname,"/tmp/%s",filename);
	if((file = fopen(filename, "r")) == NULL) {
		printf("Error: unable to open input file.\n");
		return;
	}
	if((tfile = fopen(tempname, "w")) == NULL) {
		printf("Error: unable to open output file.\n");
		fclose(file);
		return;
	}
	errno = 0;
	while((c = fgetc(file)) != EOF) {
		if(c == 0x0D) {
			continue;
		}
		if(fputc(c,tfile) == EOF) {
			printf("Unable to write to output file\n");
			fclose(file);
			fclose(tfile);
			return;
		}
		errno = 0;
	}
	if(errno != 0) {
		printf("Writing failed on output file.\n");
		fclose(file);
		fclose(tfile);
		return;
	}
	fclose(file);
	fclose(tfile);

	if((tfile = fopen(tempname,"r")) == NULL) {
		printf("Error: unable to open temp file for reading.\n");
		return;
	}
	if((file = fopen(filename,"w")) == NULL) {
		printf("Error: unable to open output file for writing.\n");
		fclose(tfile);
		return;
	}
	errno = 0;
	while((c = fgetc(tfile)) != EOF) {
		if(fputc(c,file) == EOF) {
			printf("Unable to write to output file.\n");
			fclose(file);
			fclose(tfile);
			return;
		}
		errno = 0;
	}
	if(errno != 0) {
		printf("Reading failed on input file.\n");
		fclose(file);
		fclose(tfile);
		return;
	}
	fclose(file);
	fclose(tfile);
	printf("File written successfully!\n");
}