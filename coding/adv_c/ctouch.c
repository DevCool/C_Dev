#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define MAXBUF 512
#define MAXLINES 80

int getln(char *str,size_t size);
void writefile(const char *filename,char *s);
void writemain(const char *filename,char prototypes[][MAXBUF],int ptcnt);
void writefunc(const char *filename,char prototypes[][MAXBUF],int ptcnt);

int main(int argc,char *argv[]) {
    FILE *file;
    char s[MAXBUF];
    char s2[MAXLINES][MAXBUF];
    char c;
    int i, j;

    if(argc != 2) {
        fprintf(stderr,"Usage: %s source.c\n",argv[0]);
        return 0;
    }
    printf("C Touch - Create a C language template.\n\n");
    printf("To stop entering includes send EOF on\nblank line.\n");
    printf("Enter include files:\n");
    while(1) {
        i = getln(s,sizeof(s));
        if(i <= 0)
            break;
        writefile(argv[1],s);
    }

    printf("\n\nTo stop entering function headers send EOF on\nblank line.\n");
    printf("Enter function headers:\n");
    j = 0;
    while(1) {
        i = getln(s,sizeof(s));
        if(i <= 0)
            break;
        if(j < MAXLINES) {
            strncpy(s2[j],s,MAXBUF);
            writefile(argv[1],s);
            ++j;
        }
    }
    writemain(argv[1],s2,j);
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

void writefile(const char *filename,char *s) {
    FILE *fp;
    int res;

    fp = fopen(filename,"at");
    if(fp == NULL) {
        fprintf(stderr, "Error: no such file.\n");
        return;
    }
    res = fprintf(fp, "%s", s);
    if(res < 0) {
        fprintf(stderr, "Error: cannot write to file.\n");
        return;
    }
    fputc('\n',fp);
    fclose(fp);
    puts("File was appended to.");
}

#define MAINFUNC\
    "int main(int argc, char *argv[]) {\n"\
    "\treturn 0;\n}"

void writemain(const char *filename,char prototypes[][MAXBUF],int ptcnt) {
    char bfile[256];
    FILE *fp;
    int res;

    fp = fopen(filename,"at");
    if(fp == NULL) {
        fprintf(stderr,"Error: no such file.\n");
        return;
    }
    res = fprintf(fp,"%s",MAINFUNC);
    if(res < 0) {
        fprintf(stderr,"Error: cannot write to file.\n");
        return;
    }
    fputc('\n',fp);
    puts("main() was written to file.");
    fclose(fp);
    writefunc(filename,prototypes,ptcnt);
}

void strip(char *s) {
    while(*s++ != 0)
        if(*s == ';') {
            *s = 0;
            break;
        } else {
            continue;
        }
}

void writefunc(const char *filename,char prototypes[][MAXBUF],int ptcnt) {
    FILE *fout;
    int i;

    if((fout = fopen(filename,"at")) == NULL) {
        fprintf(stderr,"Error: cannot open output file.\n");
        return;
    }
    i = 0;
    fputc('\n',fout);
    while(i < ptcnt) {
        strip(prototypes[i]);
        fprintf(fout,"%s {\n}\n\n",prototypes[i]);
        fputc('\n',fout);
        ++i;
    }
    fclose(fout);
    fputc('\n',fout);
    puts("Functions written to file.");
}

