#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "debug.h"

int main(int argc, char *argv[]) {
	FILE *fin = NULL;

	FOPEN_ERROR(fin, "test.txt", "rt");
	FCLOSE_ERROR(fin);

	return 0;

error:
	return -1;
}
