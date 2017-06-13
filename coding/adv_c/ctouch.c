/*******************************************************************
 * ctouch.c - simple program to make a template C source file.
 * by Philip "5n4k3" Simonson.
 *******************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define MAXBUF 2
#define MAXLINES 80

char *getln(size_t *i);
void writeln(const char *filename);
void writefile(const char *filename,char *s);
void writemain(const char *filename,char *prototypes[],int ptcnt);
void writefunc(const char *filename,char *prototypes[],int ptcnt);

int main(int argc,char *argv[]) {
    FILE *file;
    char *s, *s2[MAXLINES];
    size_t i;
    int j;

    if(argc != 2) {
        fprintf(stderr,"Usage: %s source.c\n",argv[0]);
        return 0;
    }
    printf("C Touch - Create a C language template.\n\n");
    printf("To stop entering includes send EOF on\nblank line.\n");
    printf("Enter include files:\n");
    while(1) {
        s = getln(&i);
        if(s == NULL)
            exit(1);
        if(i <= 0)
            break;
        writefile(argv[1],s);
    }
    writeln(argv[1]);

    printf("\n\nTo stop entering function headers send EOF on\nblank line.\n");
    printf("Enter function headers:\n");
    j = 0;
    while(1) {
        s = getln(&i);
        if(s == NULL)
            exit(1);
        if(i <= 0)
            break;
        if(j < MAXLINES) {
            s2[j] = s;
            writefile(argv[1],s2[j]);
            ++j;
        }
    }
    writeln(argv[1]);
    writemain(argv[1],s2,j);
    for(i = 0; i < j; ++i)
        free(s2[i]);
    return 0;
}

char *getln(size_t *i) {
    size_t pos, size;
    char *str;
    char c;

    size = MAXBUF*sizeof(char);
    str = malloc(size);
    if(str != NULL) {
        pos = 0;
        while(1) {
            c = getchar();
            if(c == EOF || c == 0x0A) {
                *(str+pos) = 0x00;
                *i = pos;
                return str;
            } else {
                *(str+pos) = c;
            }
            ++pos;

            if(pos >= size) {
                size += MAXBUF*sizeof(char);
                str = realloc(str,size);
                if(str == NULL)
                    break;
            }
        }
    }
    puts("Out of memory.");
    return NULL;
}

void writeln(const char *filename) {
    FILE *fp;
    int res;

    fp = fopen(filename,"at");
    if(fp == NULL) {
        fprintf(stderr, "Error: cannot open file.\n");
        return;
    }
    res = fputc('\n',fp);
    if(res < 0) {
        fprintf(stderr, "Error: cannot write newline to file.\n");
        fclose(fp);
        return;
    }
    fclose(fp);
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
}

#define MAINFUNC\
    "int main(int argc, char *argv[]) {\n"\
    "\treturn 0;\n}"

void writemain(const char *filename,char *prototypes[],int ptcnt) {
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

char *strip(char *s) {
    int spn;
    if(s == NULL)
        return NULL;
    spn = strcspn(s,";");
    s[spn] = 0;
    return s;
}

char *datatype(const char *s) {
    char tmp[128], *s2;
    int spn;
    if(s == NULL)
        return NULL;
    spn = strcspn(s," ");
    memset(tmp,0,sizeof(tmp));
    memcpy(tmp,s,spn);
    s2 = tmp;
    return s2;
}

void writefunc(const char *filename,char *prototypes[],int ptcnt) {
    FILE *fout;
    char *dtype;
    int i;

    if((fout = fopen(filename,"at")) == NULL) {
        fprintf(stderr,"Error: cannot open output file.\n");
        return;
    }
    i = 0;
    fputc('\n',fout);
    while(i < ptcnt) {
        strip(prototypes[i]);
        dtype = datatype(prototypes[i]);
        if(strcmp(dtype,"int") == 0)
            fprintf(fout,"%s {\n\treturn 0;\t/* returns 0 for success */\n}\n\n",prototypes[i]);
        else if(strcmp(dtype,"void") == 0)
            fprintf(fout,"%s {\n}\n\n",prototypes[i]);
        else if(strcmp(dtype,"char") == 0)
            fprintf(fout,"%s {\n\treturn -1;\t/* returns -1 (EOF) */\n}\n\n",prototypes[i]);
        else if(strcmp(dtype,"char*") == 0)
            fprintf(fout,"%s {\n\treturn NULL;\t/* returns NULL char pointer */\n}\n\n",prototypes[i]);
        else
            fprintf(fout,"%s {\n}\n\n",prototypes[i]);
        ++i;
    }
    fclose(fout);
    puts("Functions written to file.");
}

