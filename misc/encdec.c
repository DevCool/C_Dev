#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>

#define SECRET_PHRASE "4pPl3s#4u"

struct Header {
	char tagHeader[3];
	long size;
};

void encrypt(FILE* fp, struct Header* fHead, const char* filename, const char* outname);
void decrypt(FILE* fp, struct Header* fHead, const char* filename, const char* outname);

int main(int argc, char* argv[]) {
	FILE *fp = NULL;
	struct Header fileHead;

	if (argc < 4 || argc > 4) {
		fprintf(stderr, "Usage: %s -[ed] filename.in filename.out\n",
			argv[0]);
		exit(1);
	}

	memset(&fileHead, 0, sizeof(struct Header));
	if (argv[1][0] == '-') {
		if (argv[1][1] == 'e') {
			encrypt(fp, &fileHead, argv[2], argv[3]);
		} else if (argv[1][1] == 'd') {
			decrypt(fp, &fileHead, argv[2], argv[3]);
		} else {
			fprintf(stderr,
				"Usage: %s -[ed] file.in file.out\n",
				argv[0]);
			exit(1);
		}
	} else {
		fprintf(stderr, "Usage: %s -[ed] file.in file.out\n",
			argv[0]);
	}
}

void encrypt(FILE* fp, struct Header* fHead, const char* filename, const char* outname) {
	FILE *fout = NULL;
	char *fname = strdup(filename);
	char *fname2 = strdup(outname);
	int pos;
	char c;
	long size;

	if ((fp = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error: Cannot open file %s.\n", fname);
		free(fname2);
		free(fname);
		exit(1);
	}

	if ((fout = fopen(fname2, "wb")) == NULL) {
		fprintf(stderr, "Error: Cannot open file %s.\n", fname2);
		fclose(fp);
		free(fname2);
		free(fname);
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	strncpy(fHead->tagHeader, "PRS", 3);
	fHead->size = (size+sizeof(struct Header));

	int rc = fwrite(fHead, sizeof(struct Header), 1, fout);
	if (rc != 1) {
		fprintf(stderr, "Error: couldn't write file header.\n");
		fclose(fout);
		fclose(fp);
		free(fname2);
		free(fname);
		exit(1);
	}

	pos = 0;
	int n = 0;
	while (pos < size) {
		c = fgetc(fp);
		c ^= SECRET_PHRASE[n * sizeof(char) % sizeof(SECRET_PHRASE)] & 0xf0;
		fputc(c, fout);
		++pos;
		++n;
		if (n >= sizeof(SECRET_PHRASE)) n = 0;
	}
	fclose(fout);
	fclose(fp);
	free(fname2);
	free(fname);
}

void decrypt(FILE* fp, struct Header* fHead, const char* filename, const char* outname) {
	FILE *fout = NULL;
	char *fname = strdup(filename);
	char *fname2 = strdup(outname);
	int pos;
	char c;
	long size;

	if ((fp = fopen(fname, "rb")) == NULL) {
		fprintf(stderr, "Error: Cannot open file %s.\n", fname);
		free(fname2);
		free(fname);
		exit(1);
	}

	if ((fout = fopen(fname2, "wb")) == NULL) {
		fprintf(stderr, "Error: Cannot open file %s.\n", fname2);
		fclose(fp);
		free(fname2);
		free(fname);
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	int rc = fread(fHead, sizeof(struct Header), 1, fp);
	if (rc != 1) {
		fprintf(stderr, "Error: File format not recognized!\n");
		fclose(fout);
		fclose(fp);
		free(fname2);
		free(fname);
		exit(1);
	}

	if (strncmp(fHead->tagHeader, "PRS", 3) != 0) {
		fprintf(stderr, "Error: File Header invalid.\n");
		fclose(fout);
		fclose(fp);
		free(fname2);
		free(fname);
		exit(1);
	}

	if (size != fHead->size) {
		fprintf(stderr, "Error: File size is invalid.\n");
		fclose(fout);
		fclose(fp);
		free(fname2);
		free(fname);
		exit(1);
	}

	pos = 0;
	int n = 0;
	while (pos < (size-sizeof(struct Header))) {
		c = fgetc(fp);
		c ^= SECRET_PHRASE[n * sizeof(char) % sizeof(SECRET_PHRASE)] & 0xf0;
		fputc(c, fout);
		++pos;
		++n;
		if (n >= sizeof(SECRET_PHRASE)) n = 0;
	}
	fclose(fout);
	fclose(fp);
	free(fname2);
	free(fname);
}
