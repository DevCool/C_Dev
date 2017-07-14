#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define DEBUGGING
#include "debug.h"

#ifdef __linux
extern int fileno(FILE *fp);
extern int ftruncate(int fildes, size_t size);
#endif

int file_shredder(FILE *fp, unsigned char mode);

int main(int argc, char *argv[]) {
  FILE *fp = NULL;

  if(argc < 2 || argc > 3) {
    printf("Usage: %s -[uz] <infile>\n", argv[0]);
    return 1;
  }

  if(argc == 2) {	/* handle shred without args */
    FOPEN_ERROR(fp, argv[1], "r+b");
    ERROR_FIXED(file_shredder(fp, 1) == EOF, "File shredder failed.");
    FCLOSE_ERROR(fp);
    remove(argv[1]);
    puts("File shredded!");
  } else {	/* handle shred with args */
    if(argv[1][0] == '-') {
      switch(argv[1][1]) {
      case 'u':
	FOPEN_ERROR(fp, argv[2], "r+b");
	if(argv[1][2] == 'z')
	  ERROR_FIXED(file_shredder(fp, 1) == EOF, "File shredder failed.");
	ERROR_FIXED(file_shredder(fp, 0) == EOF, "File shredder failed.");
	FCLOSE_ERROR(fp);
	remove(argv[2]);
	puts("File shredded!");
	break;
      case 'z':
	FOPEN_ERROR(fp, argv[2], "r+b");
	ERROR_FIXED(file_shredder(fp, 1) == EOF, "File shredder failed.");
	if(argv[1][2] == 'u')
	  ERROR_FIXED(file_shredder(fp, 0) == EOF, "File shredder failed.");
	FCLOSE_ERROR(fp);
	remove(argv[2]);
	puts("File shredded!");
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
      bytesWritten = fwrite(buf, 1, CHUNK_SIZE-total_bytes, fp);
      if(bytesWritten) total_bytes += bytesWritten;
      else break;
    }
    ERROR_FIXED(total_bytes == file_size, "Failed to zero file.");
    puts("File was zeroed!");
  } else {
#if defined(_WIN32) || (_WIN64)
    ERROR_FIXED(_chsize(fileno(fp), 64) != 0, "Failed to truncate file.");
#else
    ERROR_FIXED(ftruncate(fileno(fp), 64) != 0, "Failed to truncate file.");
#endif
    puts("File was truncated!");
  }
  return 0;

error:
  return -1;
}
