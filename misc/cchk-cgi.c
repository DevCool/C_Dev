#include "common/cgi_helper.h"

#define MAXVAL	1024
#define MAXLINE	1024

struct STACK {
	int top;
	int val[MAXVAL];
	int pos[MAXVAL];
};
typedef struct STACK stack_t;

enum STATE {
	CODE,
	COMMENT,
	QUOTES
};
typedef enum STATE state_t;

/* global variables */
char line[MAXLINE];			/* global line variable */
stack_t stack;				/* stack variable */
state_t state;				/* state variable */
int comments;				/* comment count */
int quotes;					/* quotes count */
int error;					/* error flag */

/* stack functions */
int is_empty(stack_t *stack);
int is_full(stack_t *stack);
int push(stack_t *stack, int val, int pos);
int pop(stack_t *stack, int *val, int *pos);

/* program functions */
int getln(void);
int check_source(void);

/* program to check the source code for errors */
main(int argc, char **argv, char **envp)
{
	check_source();
	return 0;
}

/* is_empty:  checks if stack is empty */
int is_empty(stack_t *stack)
{
	return (stack->top == 0);
}

/* is_full:  checks if stack is full */
int is_full(stack_t *stack)
{
	return (stack->top == MAXVAL-1);
}

/* push:  push values onto stack */
int push(stack_t *stack, int val, int pos)
{
	if (is_full(stack)) {
		printf("Stack overflow: already at maximum value.\n");
		return -1;
	}
	stack->val[stack->top] = val;
	stack->pos[stack->top] = pos;
	stack->top++;
	return 0;
}

/* pop:  pop values from stack */
int pop(stack_t *stack, int *val, int *pos)
{
	if (is_empty(stack)) {
		printf("Stack is empty.\n");
		return -1;
	}
	stack->top--;
	*val = stack->val[stack->top];
	*pos = stack->pos[stack->top];
	return 0;
}

/* getln:  get line of input from stdin */
int getln(void)
{
	extern char line[];
	int c, i;
	for (i = 0; i < MAXLINE-2 && (c = getchar()) != EOF && c != '\n'; i++)
		line[i] = c;
	if (c == '\n')
		line[i++] = '\n';
	line[i] = '\0';
	return i;
}

/* special_body:  prints output for syntax checker */
void body(void)
{
	extern stack_t stack;
	extern state_t state;
	extern int comments;
	extern int quotes;
	extern int error;

	if (comments > 0 || quotes > 0) {
		printf("<p>Comments: %d<br />"
			"Quotes: %d</p>",
			comments, quotes);
	}
	if (state == COMMENT) {
		printf("<p>Code ends inside of comment.</p>");
		error = 1;
	} else if (state == QUOTES) {
		printf("<p>Code ends inside of quotes.</p>");
		error = 1;
	} else if (is_empty(&stack) && error == 0) {
		printf("<p>Code seems to be okay.</p>");
	} else {
		while (!is_empty(&stack)) {
			int val, pos;
			pop(&stack, &val, &pos);
			printf("<p>Syntax error: line %d : '%c' doesn't have match.</p>",
				pos, val);
		}
		printf("<p>There were errors in the code.</p>");
	}
}

/* check_source:  checks source code for syntax errors */
int check_source(void)
{
	extern char line[];
	extern stack_t stack;
	extern state_t state;
	extern int comments;
	extern int quotes;
	extern int error;
	int c, d, i;
	int ln, len;

	error = 0;
	ln = 0;
	comments = quotes = 0;
	while ((len = getln()) > 0) {
		++ln;
		i = 0;
		while (i < len && (c = line[i++]) != EOF) {
			switch (state) {
			case CODE:
				if (c == '\'' || c == '"') {
					state = QUOTES;
					break;
				}
				if (c == '/') {
					d = line[i++];
					if (d == '*') {
						state = COMMENT;
						break;
					}
				}
				if (c == '(' || c == '[' || c == '{') {
					push(&stack, c, ln);
				} else if (c == ')' || c == ']' || c == '}') {
					if (is_empty(&stack)) {
						printf("<p>Syntax error line %d: '%c' doesn't have match.<p>",
							ln, c);
						error = 1;
					} else {
						int val, pos;
						pop(&stack, &val, &pos);
						if ((c == ')' && val != '(') ||
							(c == ']' && val != '[') ||
							(c == '}' && val != '{')) {
							printf("<p>Syntax error line %d: '%c' does not match"
								" '%c'.</p>", pos, val, c);
							error = 1;
						}
					}
				}
			break;
			case QUOTES:
				if (c == '\\')
					continue;
				else if (c == '\'' || c == '"') {
					quotes++;
					state = CODE;
				}
			break;
			case COMMENT:
				if (c == '*') {
					d = line[i++];
					if (d == '/') {
						comments++;
						state = CODE;
					}
				}
			break;
			}
		}
	}

	setup(header_default, body, footer_default);
	return 0;
}
