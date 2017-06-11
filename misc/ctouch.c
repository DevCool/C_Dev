#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define MAXBUF 512

int getln();
int create_func();

int main(argc,argv)
	int argc;
	char **argv;
{
	int i;

	i = 0;
	if(argc < 3)
		printf("Usage: %s <filename.ext>\n",argv[0]);
	else {
		i = create_func(argv[1],atoi(argv[2]));
		printf("File %s created.\n"
			"Created %d function(s).\n",
			argv[1],i);
	}
	return 0;	
}

int getln(str,size)
	char str[];
	size_t size;
{
	int c,i;

	if(str == NULL)
		return -1;
	i = 0;
	c = 0;
	memset(str,0,MAXBUF);
	while(c != '\n') {
		c = getchar();
		str[i++] = c;
	}
	str[i] = 0;
	fflush(stdin);
	return i;
}

/* signed data types */
#define TYPE_CHAR "char"
#define TYPE_SHORT "short"
#define TYPE_INT "int"
#define TYPE_LONG "long int"
#define TYPE_LONGD "long double"
#define TYPE_LONGL "long long int"
#define TYPE_FLOAT "float"
#define TYPE_DOUBLE "double"

int create_func(filename,cnt)
	const char *filename;
	int cnt;
{
	FILE *file;
	char s[MAXBUF];
	int c,i;

	if((file = fopen(filename,"wt")) == NULL) {
		printf("Error: Cannot open file for writing.\n");
		return -1;
	}
	for(i = 0; i < cnt; ++i) {
		printf("Enter function %d name: ",i);
		getln(s,sizeof(s));
		printf("Enter func type (0,1,2,3,4,5,6)\n"
			"0 - char\n1 - short\n2 - int\n"
			"3 - long int\n4 - long long int\n"
			"5 - float\n6 - double\n"
			"> ");
		fflush(stdin);
		c = getchar();
		getchar();

		switch(c) {
			case 0:
				fprintf(file,"%s %s()\n",TYPE_CHAR,s);
			break;
			case 1:
				fprintf(file,"%s %s()\n",TYPE_SHORT,s);
			break;
			case 2:
				fprintf(file,"%s %s()\n",TYPE_INT,s);
			break;
			case 3:
				fprintf(file,"%s %s()\n",TYPE_LONG,s);
			break;
			case 4:
				fprintf(file,"%s %s()\n",TYPE_LONGL,s);
			break;
			case 5:
				fprintf(file,"%s %s()\n",TYPE_FLOAT,s);
			break;
			case 6:
				fprintf(file,"%s %s()\n",TYPE_DOUBLE,s);
			break;
			default:
				fprintf(file,"%s %s()\n",TYPE_INT,s);
		}
		fprintf(file,"{\n}\n");
	}
	fclose(file);
	return i;
}
