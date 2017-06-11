#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <ctype.h>

#define NDEBUG

#define BUFSIZE 1
#define NEWLINE 2

int p_strcmp(const char *a, const char *b)
{
	while(*a != 0) {
		if(*a < *b)
			return (int)*b-*a;
		else if(*a > *b)
			return (int)*a-*b;
		++a;
		++b;
	}
	return 0;
}

int p_stricmp(const char *a, const char *b)
{
	while(*a != 0) {
		if(tolower(*a) < tolower(*b))
			return (int)*b-*a;
		else if(tolower(*a) > tolower(*b))
			return (int)*a-*b;
		++a;
		++b;
	}
	return 0;
}

char *get_string(char a, const char *b)
{
	int pos = 0;
	int size = BUFSIZE;
	char *buffer = NULL;
	int c;

	buffer = realloc(buffer, size);
	if(buffer == (void*)0)
		return NULL;

	printf("%s", b);

	while(1) {
		c = getchar();
		if(c == -1 || c == '\n') {
			if(a) {
				*(buffer+pos) = '\n';
				*(buffer+pos+1) = 0;
			}
			else if(!a) {
				*(buffer+pos) = 0;
			}
			return buffer;
		}
		else {
			*(buffer+pos) = c;
		}
		++pos;

		if(pos <= size && a) {
			size += NEWLINE;
			buffer = realloc(buffer, size);
			if(!buffer)
				break;
		}
		else if(pos <= size && a == 0) {
			size += 1;
			buffer = realloc(buffer, size);
			if(!buffer)
				break;
		}
		else if(pos >= size) {
			size += BUFSIZE;
			buffer = realloc(buffer, size);
			if(!buffer)
				break;
		}
	}

	printf("ERROR : [%p] : Out of memory!\n", buffer);
	return NULL;
}

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#define SECRET_CODE "TuX3d0\0"
#define SECRET_MSG "                                     " \
		"           Perfect MATCH ! ! !       " \
		"                                     "

void change_attr(WORD wAttr)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, wAttr);
}

int main()
{
#if defined(TESTING)
	char *input = NULL;
	input = get_string(1, "Enter some text below >>\n");
	printf("You entered:\n%s", input);
	if(input != NULL) free(input);
	input = NULL;
#endif
	char *input2 = NULL;
	input2 = get_string(0, "Enter the code: ");
	if(p_strcmp(input2, SECRET_CODE) == 0) {
		system("cls");
		change_attr(0x001E);
		puts(SECRET_MSG);
	}
	else
		if(p_stricmp(input2, SECRET_CODE) == 0)
			puts("match!");
		else
			puts("doesn't match!");
#if !defined(NDEBUG)
	printf("You entered: %s", input2);
#endif
	if(input2 != NULL) free(input2);
	input2 = NULL;
	change_attr(0x0007);

	return 0;
}
