#include <stdio.h>

#define MAXLINE 1000

char line[MAXLINE];

typedef enum STATE {
	CODE,
	QUOTE,
	COMMENT
} state_t;

/* getline:  gets a line of input from user */
int get_line(void);

main()
{
	extern char line[];
	int parens;
	int brackets;
	int braces;
	int c, d, i, len;
	int error;
	state_t state;

	error = 0;
	state = CODE;
	braces = brackets = parens = 0;
	while ((len = get_line()) > 0) {
		switch (state) {
			case CODE:
			c = line[i];
			if (c == '\'' || c == '"') {
				state = QUOTE;
			} else if (c == '/') {
				d = line[++i];
				if (d == '*') {
				state = COMMENT;
				} else {
					--i;
				}
			} else if (c == '(') {
				parens++;
			} else if (c == '[') {
				brackets++;
			} else if (c == '{') {
				braces++;
			} else if (c == ')') {
				parens--;
			} else if (c == ']') {
				brackets--;
			} else if (c == '}') {
				braces--;
			}
			break;
			case COMMENT:
			if (c == '*') {
				d = line[++i];
				if (d == '/')
					state = CODE;
				else
					--i;
			}
			break;
			case QUOTE:
			if (c == '\\')
				++i;
			else if (c == '\'' || c == '"')
				state = CODE;
			break;
		}
		i++;
	}
	if (state == COMMENT) {
		printf("Code ends inside of comment.\n");
		error = 1;
	}
	if (state == QUOTE) {
		printf("Code ends inside of quotes.\n");
		error = 1;
	}
	if (parens < 0 || parens > 0) {
		printf("Unbalanced parenthesises.\n");
		error = 1;
	} else if (braces < 0 || braces > 0) {
		printf("Unbalanced braces.\n");
		error = 1;
	} else if (brackets < 0 || brackets > 0) {
		printf("Unbalanced brackets.\n");
		error = 1;
	}
	if (state == CODE && error == 0)
		printf("Code seems okay.\n");
	if (error == 1)
		printf("Code has errors.\n");
}

/* getline:  gets user input from stdin */
int get_line(void)
{
	extern char line[];
	int c, i;

	for (i = 0; i < MAXLINE-2 && (c = getchar()) != EOF && c != '\n'; i++)
		line[i] = c;
	if (c == '\n')
		line[i++] = c;
	line[i] = '\0';
	return i;
}
