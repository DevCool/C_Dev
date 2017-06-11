#include <stdio.h>
#include <stddef.h>
#include <string.h>

#define MAXBUF 512

int getln(char *str,size_t size);
void writefile(FILE *fp,char *s);

int main(int argc,char *argv[]) {
    FILE *file;
    char s[MAXBUF];
    char c;
    int i;

    if(argc != 2) {
        fprintf(stderr,"Usage: %s source.c\n",argv[0]);
        return 0;
    }
    if((file = fopen(argv[1],"at")) == NULL) {
        fprintf(stderr,"Error: could not create source file.\n");
        return 1;
    }
    printf("C Touch - Create a C language template.\n\n");
    printf("To stop entering includes send EOF on\nblank line.\n");
    printf("Enter include files:\n");
    while(1) {
        i = getln(s,sizeof(s));
        if(i <= 0)
            break;
        writefile(file,s);
    }

    printf("\n\nTo stop entering function headers send EOF on\nblank line.\n");
    printf("Enter function headers:\n");
    while(c != 'n' || c != 'N') {
        i = getln(s,sizeof(s));
        if(i <= 0)
            break;
        writefile(file,s);
    }
    fclose(file);
    return 0;
}

int getln(char *str,size_t size) {
    int c,i,j;

    if(str == NULL)
    return -1;
    j = 0;
    memset(str,0,size);
    for(i = 0; (c = getchar()) != EOF && c != 0x0D; ++i)
	if(i < size-1) {
		str[i] = c;
		++j;
	}
    if(c == 0x0D) {
	str[i] = c;
	++i;
	++j;
    }
    ++j;
    str[j] = 0;
    return i;
}

void writefile(FILE *fp,char *s) {
    int res;

    if(fp == NULL) {
        fprintf(stderr, "Error: no such file.\n");
        return;
    }
    res = fprintf(fp, "%s", s);
    if(res < 0) {
        fprintf(stderr, "Error: cannot write to file.\n");
        return;
    }
    puts("File was appended to.");
}

