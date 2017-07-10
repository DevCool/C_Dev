/*******************************************************************
 * ctouch.c - simple program to make a template C source file.
 * by Philip "5n4k3" Simonson.
 *******************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

#define MAXBUF 2
#define MAXLINES 80
#define MAXPATH 512

char *getln(size_t *i);
void writeln(const char *filename);
void writefile(const char *filename,char *s);
void writemain(const char *filename);
void writefunc(const char *filename,char *prototypes[],int ptcnt);
void writemake(int argc,char** argv[]);

int main(int argc,char *argv[]) {
    FILE *file;
    char *s, *s2[MAXLINES];
    size_t i;
    int j, k;

    if(argc < 2) {
        fprintf(stderr,"Do NOT put file extension.\nUsage: %s file1 [...]\n",argv[0]);
        return 0;
    }
    for(k = 1; k < argc; ++k) {
        char line[512];
	memset(line, 0, sizeof(line));
        mkdir("bin");
        mkdir("obj");
        mkdir("src");
        printf("\n\n**************** %s ******************\n\n",argv[k]);
        printf("C Touch - Create a C language template.\n\n");
        printf("To stop entering includes send EOF on\nblank line.\n");
        printf("Enter include files:\n");
        snprintf(line, sizeof(line), "src/%s", argv[k]);
        while(1) {
            s = getln(&i);
            if(s == NULL)
                exit(1);
            if(i <= 0)
                break;
            writefile(line,s);
        }
        writeln(line);

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
                writefile(line,s2[j]);
                ++j;
            }
        }
        writeln(line);
        if(strstr(line,"main") != NULL)
            writemain(line);
        writefunc(line,s2,j);
        for(i = 0; i < j; ++i)
            if(s2[i] != NULL)
                free(s2[i]);
    }
    writemake(argc,&argv);
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

void writemain(const char *filename) {
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

void writemake(int argc,char **argv[]) {
    FILE *fout;
    char path[MAXPATH];
    char *progname;
    int i, res;
    size_t namelen;

    memset(path,0,sizeof(path));
    if(getcwd(path,sizeof(path)) == NULL) {
        fprintf(stderr, "Cannot get current directory.\n");
        return;
    }
    strncat(path,"/Makefile",MAXPATH);
    if((fout = fopen(path,"wt")) == NULL) {
        fprintf(stderr, "Cannot open makefile for output.\n");
        return;
    }
    printf("Name your project: ");
    progname = getln(&namelen);
    res = fprintf(fout,"CC=gcc\nCFLAGS?=-std=c89 -Wall -Wextra -pedantic -g\n"
            "LDFLAGS?=-g\nLIBS=\n\nSRCDIR=src\nBINDIR=bin\nOBJDIR=obj\n\n"
	    "SRC=$(wildcard $(SRCDIR)/*.c)\nTARGET=%s\n\n", progname);
    free(progname);
    if(res == -1) {
        fclose(fout);
        remove(path);
        fprintf(stderr, "Makefile failed to write so it was removed.\n");
        return;
    }
    res = fprintf(fout,"OBJ=$(SRC:$(SRCDIR)/%%.c=$(OBJDIR)/%%.c.o)\n\nall: $(BINDIR)/$(TARGET)\n\n"
	    "$(OBJ) : $(OBJDIR)/%%.c.o: $(SRCDIR)/%%.c\n\t"
            "@$(CC) $(CFLAGS) -c -o $@ $<\n\t@echo Compiled $< successfully!\n\n"
            "$(BINDIR)/$(TARGET): $(OBJ)\n\t"
            "@echo Linking executable...\n\t"
            "@$(CC) $(LDFLAGS) $^ -o $@ $(LIBS)\n\t@echo Executable built with success!\n\n"
	    ".PHONY: clean clean-windows\n\nclean:\n\t@echo"
            "Cleaning project...\n\t@rm -f $(OBJDIR)/*.o $(BINDIR)/$(TARGET)\n\t@echo "
            "Cleanup complete!\n\nclean-windows:\n\t@echo Cleaning project...\n\t"
	    "@del $(OBJDIR)\*.o $(BINDIR)\$(TARGET).exe\n\t@echo Cleanup complete...\n");
    if(res == -1) {
        fclose(fout);
        remove(path);
        fprintf(stderr, "Makefile failed to write so it was removed.\n");
        return;
    }
    fclose(fout);
    puts("Makefile was written successfully.");
}