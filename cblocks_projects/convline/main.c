#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

int
main(argc, argv)
int argc;
char **argv;
{
    char fname[128];
    FILE *fin = NULL;
    FILE *fout = NULL;
    int c;
    size_t size;

    if(argc < 2) {
        printf("Usage: convline <filein.txt>\n");
        return 1;
    }
    memset(fname, 0, sizeof(fname));
    strcpy(fname, argv[1]);
    fin = fopen(fname, "rt");
    fout = fopen("data.txt", "wt");
    size = ftell(fin);
    rewind(fin);
    while((c = fgetc(fin)) != -1) {
        long i;
        for(i=0; i<size; ++i) {
            if(c != 0x0A)
                fputc(c, fout);
            putchar(c);
        }
    }
    if(c == -1)
        printf("File was written.\n");
    else
        printf("Error: file couldn't be written.\n");
    fclose(fin);
    fclose(fout);
    return 0;
}
