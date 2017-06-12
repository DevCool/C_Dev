#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define MAXBUF 512

int getln(char *str,size_t size);
void writefile(const char *filename,char *s);
void writemain(const char *filename);
void writefunc(const char *filename);

int main(int argc,char *argv[]) {
    FILE *file;
    char s[MAXBUF];
    char c;
    int i;

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
    while(1) {
        i = getln(s,sizeof(s));
        if(i <= 0)
            break;
        writefile(argv[1],s);
    }
    writemain(argv[1]);
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
    memset(bfile,0,sizeof(bfile));
    sprintf(bfile,"cp %s %s.old",filename,filename);
    system(bfile);
    writefunc(filename);
}

void writefunc(const char *filename) {
    FILE *fp,*fout;
    char buf[512];
    char *tmp;
    int c;

    if((fp = fopen(filename,"rt")) == NULL) {
        fprintf(stderr,"Error: cannot open file.\n");
        return;
    }
    if((fout = fopen("tmpfile.txt","at")) == NULL) {
        fprintf(stderr,"Error: cannot open output file.\n");
        return;
    }
    memset(buf,0,sizeof(buf));
    while(fgets(buf,sizeof(buf),fp) != NULL)
        fprintf(fout,"%s",buf);
    rewind(fp);
    memset(buf,0,sizeof(buf));
    while(fgets(buf,sizeof(buf),fp) != NULL) {
        if(strncmp(buf,"int main",8) == 0) {
            break;
        } else if(strncmp(buf,"#include",8) == 0) {
            continue;
        }
        tmp = strtok(buf,";");
        if(tmp != NULL) {
            fprintf(fout,"%s {\n}\n\n",tmp);
            tmp = NULL;
        }
    }
    fputc('\n',fout);
    fclose(fp);
    fclose(fout);
    if((fp = fopen("tmpfile.txt","rt")) == NULL) {
        fprintf(stderr,"Error: cannot open file for reading.\n");
        return;
    }
    if((fout = fopen(filename,"wt")) == NULL) {
        fprintf(stderr,"Error: cannot open file for writing.\n");
        fclose(fp);
        return;
    }
    while((c = fgetc(fp)) != EOF) {
        fputc(c,fout);
    }
    fclose(fp);
    fclose(fout);
    remove("tmpfile.txt");
    puts("Functions written to file.");
}

