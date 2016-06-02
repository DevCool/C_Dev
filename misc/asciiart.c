/*****************************************
 * Coded by Philip "tru3c0d3r88" Simonson
 *****************************************
 * Description: Ascii Art Generator -
 * generates ascii art from bitmap image.
 *****************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned short uint16;
typedef unsigned int uint32;

typedef struct BMPFILEHEADER {
	uint16 filetype;
	uint32 filesize;
	uint32 reserved;
	uint32 offset;
	uint32 bytesInHeader;
	uint32 width;
	uint32 height;
	uint16 planes;
	uint16 bitsPerPixel;
	uint32 compression;
	uint32 size;
	uint32 horizRes;
	uint32 vertRes;
	uint32 indiciesUsed;
	uint32 indiciesImportant;
} BMPFileHeader;

#define MAX_SHADES 10
#define DEBUG 1

/* Read BMP Header */
void readHeader(FILE *fp, BMPFileHeader *header) {
	fread(&header->filetype, 2, 1, fp);
	fread(&header->filesize, 4, 1, fp);
	fread(&header->reserved, 4, 1, fp);
	fread(&header->offset, 4, 1, fp);
	fread(&header->bytesInHeader, 4, 1, fp);
	fread(&header->width, 4, 1, fp);
	fread(&header->height, 4, 1, fp);
	fread(&header->planes, 2, 1, fp);
	fread(&header->bitsPerPixel, 2, 1, fp);
	fread(&header->compression, 4, 1, fp);
	fread(&header->size, 4, 1, fp);
	fread(&header->horizRes, 4, 1, fp);
	fread(&header->vertRes, 4, 1, fp);
	fread(&header->indiciesUsed, 4, 1, fp);
	fread(&header->indiciesImportant, 4, 1, fp);

#ifdef DEBUG
	printf("filetype: %u\n", header->filetype);
	printf("filesize: %u\n", header->filesize);
	printf("reserved: %u\n", header->reserved);
	printf("offset: %u\n", header->offset);
	printf("bytesInHeader: %u\n", header->bytesInHeader);
	printf("width: %u\n", header->width);
	printf("height: %u\n", header->height);
	printf("planes: %u\n", header->planes);
	printf("bitsPerPixel: %u\n", header->bitsPerPixel);
	printf("compression: %u\n", header->compression);
	printf("size: %u\n", header->size);
	printf("horizRes: %u\n", header->horizRes);
	printf("vertRes: %u\n", header->vertRes);
	printf("indiciesUsed: %u\n", header->indiciesUsed);
	printf("indiciesImportant: %u\n", header->indiciesImportant);
#endif
}

int main(int argc, char *argv[]) {
	if (argc < 2 || argc > 3) {
		fprintf(stderr, "Usage: %s filein.bmp fileout.txt\n", argv[0]);
		return -1;
	}

	int width, height;
	int x = 0, y = 0;
	char shades[MAX_SHADES] = {'#','$','O','=','+','|','-','^','.',' '};
	int average_color = 0;
	FILE *bmpfile = NULL;
	FILE *txtfile = NULL;
	BMPFileHeader bitmapInfo;

	memset(&bitmapInfo, 0, sizeof(BMPFileHeader));
	bmpfile = fopen(argv[1], "rb");
	if (bmpfile == NULL) {
		fprintf(stderr, "Error opening bmp file.\n");
		return 1;
	}

	/* Read BMP Header */
	readHeader(bmpfile, &bitmapInfo);

	/* Read Image */
	width = (int)bitmapInfo.width;
	if (width < 0)
		width *= -1;
	height = (int)bitmapInfo.height;
	if (height < 0)
		height *= -1;
	int rowsize = width * 3; /*width * 3*/
	unsigned char *image = malloc(rowsize * height);

	rewind(bmpfile);
	fseek(bmpfile, bitmapInfo.offset, SEEK_SET);
	int rc = fread(image, 3*width*height, 1, bmpfile);
	fclose(bmpfile);

#ifdef DEBUG
	fprintf(stderr, "[DEBUG] : Bitmap size (%dx%d)\n",
		width, height);
	fprintf(stderr, "[DEBUG] : Rowsize : %d\n",
		rowsize);
#endif

	/* write the output file */
	txtfile = fopen(argv[2], "wt");
	if (txtfile == NULL) {
		fprintf(stderr, "Cannot open output file for writing.\n");
		free(image);
		return -1;
	}

	for (y = height-1; y >= 0; y--) {
		for (x = 0; x < width; x++) {
			average_color = (image[x*3+y*rowsize] +
					image[x*3+1+y*rowsize] +
					image[x*3+2+y*rowsize]) / 3;

			/* Conver to a shade */
			average_color /= (256/MAX_SHADES);
			if(average_color >= MAX_SHADES)
				average_color = MAX_SHADES-1;

			/* Output */
			fputc(shades[average_color], txtfile);
		}
		fputc('\n', txtfile);
	}
	fclose(txtfile);
	free(image);
	return 0;
}
