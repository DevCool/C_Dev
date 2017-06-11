/*************************************************************
 * printf.c - my custom fputc and fprintf functions.         *
 *************************************************************
 * Copyright (C) Philip R Simonson 2017.                     *
 *************************************************************
 */

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <winsock2.h>

int main()
{
	int _fgetc(FILE *fp);
	int _fputc(int c, FILE *fp);
	int _fprintf(FILE *fp, const char *s, ...);
	
	_fprintf(stdout, "Shit yeah!\n");
	_fprintf(stdout, "Hello %s World!\n", "man, WHAT'S Up");
	_fprintf(stdout, "Yo %s, hello.\n", "dog");
	_fprintf(stdout, "Hello, Dog.\n");
	_fprintf(stdout, "Hello %d times.\n", 5067397);
	_fprintf(stdout, "%c:%c:%c:%c\n", 65 & 0xf0 << 1, 65 & 0xf0 >> 4, 65 | 0x0f << 2, 0x41 << 2 ^ ~11);

	return 0;
}

typedef unsigned long long int size_t;

int _getche(void)
{
	struct timeval tv;
	struct fd_set readfd;
	int retval;
	int c;

	FD_ZERO(&readfd);
	FD_SET(STDIN_FILENO, &readfd);

	tv.tv_sec = 1;
	tv.tv_usec = 0;

	retval = select(1, &readfd, NULL, NULL, &tv);
	if(retval) {
		if(FD_ISSET(STDIN_FILENO, &readfd))
			c = getchar();
	} else {
		return -1;
	}
	return c;
}

char *p_itoc(int c)
{
	static char digits[24] = {0};
	static char *p = digits;
	char digit = c % 10 + 48;
	*p++ = digit;
	return (c /= 10) ? p_itoc(c) : digits;
}

char *p_rnum(char *p)
{
	static char digits[24] = {0};
	char *p2 = digits;
	int endstr = strlen(p);
	while(endstr-- > 0)
		*p2++ = *(p+endstr);
	return digits;
}

int _fgetc(FILE *fp)
{
	int res;
	int c;

	res = read(fp->_file, &c, 1);
	if(res < 0)
		return res;
	return c;
}

int _fputc(int c, FILE *fp)
{
	int res;

	res = write(fp->_file, &c, 1);
	fflush(fp);
	return res;
}

int _fprintf(FILE *fp, const char *s, ...)
{
	va_list args;
	int res;
	int c,i;

	c = 0;
	va_start(args, s);
	for(c = 0; c < strlen(s); ++c) {
		if(s[c] == '%') {
			char *p;
			int j,k;
			++c;
			switch(s[c]) {
				case 's':
					for(p = va_arg(args, char *), i = 0; i < strlen(p); ++i)
						_fputc(p[i], fp);
					p = NULL;
				break;
				case 'd':
					j = va_arg(args, int);
					if((p = p_itoc(j)) == NULL) {
						_fprintf(stderr, "Error: could not convert number to string.\n");
						continue;
					}
					p = p_rnum(p);
					while(*p != 0)
						_fputc(*p++, stdout);
				break;
				case 'c':
					k = va_arg(args, int);
					_fputc(k, stdout);
				break;
				default:
					_fprintf(stderr, "%d: needs an argument.\n", c);
			}
			continue;
		} else if(s[c] == '\\') {
			++c;
			switch(s[c]) {
				case 'n':
					res = _fputc('\n', fp);
				break;
				case 'r':
					res = _fputc('\r', fp);
				break;
				default:
					_fprintf(stderr, "%d: needs an argument.\n", c);
			}
			continue;
		}
		res = _fputc(s[c], fp);
	}
	va_end(args);

	return res;
}
