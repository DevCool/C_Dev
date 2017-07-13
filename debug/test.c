#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEBUGGING
#include "debug.h"

int file_copy(FILE *fin, FILE *fout);

int main(int argc, char *argv[]) {
	FILE *fin = NULL;
	FILE *fout = NULL;

	/* Show usage info */
	if(argc < 2 || argc > 3) {
		printf("Usage: %s <infile> <outfile>\n", argv[0]);
		return 1;
	}

	/* Show version info */
	if(argc == 2) {
		if(argv[1][0] == '-') {
			switch(argv[1][1]) {
			case 'v':
				printf("File copier v1.0 by Philip R. Simonson\n");
				return 0;
			break;
			default:
				printf("Unknown option: %c\n", argv[1][1]);
				return 2;
			}
		}
	}

	printf("Started file copy.\n");
	FOPEN_ERROR(fin, argv[1], "rt");
	FOPEN_ERROR(fout, argv[2], "wt");
	if(file_copy(fin, fout) == 0)
		printf("File copy was successful!\n");
	else
		printf("File copy was unsuccessful!\n");
	FCLOSE_ERROR(fin);
	FCLOSE_ERROR(fout);
	printf("File copy ended.");

	return 0;

error:
	FCLOSE_ERROR(fin);
	FCLOSE_ERROR(fout);
	return -1;
}

#define CHUNK_SIZE 1024

int file_copy(FILE *fin, FILE *fout) {
	char buf[CHUNK_SIZE];
	size_t bytesRead, bytesWritten;
	size_t total_bytes = 0, size = 0;

	while((bytesRead = fread(buf, 1, sizeof(buf), fin)) > 0)
		size += bytesRead;
	rewind(fin);
	while((bytesRead = fread(buf, 1, sizeof(buf), fin)) > 0) {
		bytesWritten = fwrite(buf, 1, bytesRead, fout);
		if(bytesWritten > 0)
			total_bytes += bytesWritten;
		else if(bytesWritten < 0)
			fprintf(stderr, "Failed to write %lu bytes to file.\n", bytesRead);
	}

	CHECK(total_bytes != size, "INFO", "Real size: %lu\nTotal copied: %lu",\
		size, total_bytes);
	return 0;

error:
	return -1;
}
