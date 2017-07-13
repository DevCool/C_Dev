#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEBUGGING
#include "debug.h"

int file_shredder(FILE *fp, unsigned char mode);

int main(int argc, char *argv[]) {
	FILE *fp = NULL;

	if(argc < 2 || argc > 3) {
		printf("Usage: %s -[uz] <infile>\n", argv[0]);
		return 1;
	}

	if(argc == 2) {	/* handle shred without args */
		FOPEN_ERROR(fp, argv[1], "r+b");
		ERROR(file_shredder(fp, 1) == EOF, "File shredder failed.");
		FCLOSE_ERROR(fp);
	} else {	/* handle shred with args */
		if(argv[1][0] == '-') {
			switch(argv[1][1]) {
			case 'u':
				FOPEN_ERROR(fp, argv[2], "r+b");
				if(argv[1][2] == 'z')
					ERROR(file_shredder(fp, 1) == EOF, "File shredder failed.");
				ERROR(file_shredder(fp, 0) == EOF, "File shredder failed.");
				FCLOSE_ERROR(fp);
			break;
			case 'z':
				FOPEN_ERROR(fp, argv[2], "r+b");
				if(argv[1][2] == 'u')
					ERROR(file_shredder(fp, 0) == EOF, "File shredder failed.");
				ERROR(file_shredder(fp, 1) == EOF, "File shredder failed.");
				FCLOSE_ERROR(fp);
			break;
			default:
				printf("%s: Unknown option [%c]\n", argv[0], argv[1][1]);
				return 2;
			}
		} else {
			printf("Usage: %s -[uz] <infile>\n", argv[0]);
			return 1;
		}
	}
	return 0;

error:
	FCLOSE_ERROR(fp);
	return -1;
}

#define CHUNK_SIZE 1024

int file_shredder(FILE *fp, unsigned char mode) {
	if(mode == 1) {
		char buf[CHUNK_SIZE];
		size_t file_size = 0, total_bytes = 0;
		size_t bytesRead, bytesWritten;

		while((bytesRead = fread(buf, 1, sizeof(buf), fp)) > 0)
			file_size += bytesRead;
		rewind(fp);
		DEBUG("INFO", "File size: %lu", file_size);
		while(total_bytes <= file_size) {
			memset(buf, 0, sizeof(buf));
			bytesWritten = fwrite(buf, 1, sizeof(buf), fp);
			if(bytesWritten) total_bytes += bytesWritten;
		}
		return 0;
	} else {
		printf("Not yet implemented!");
		return 0;
	}
	return -1;
}