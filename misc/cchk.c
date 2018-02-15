#include <stdio.h>

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

char line[MAXLINE];			/* global line variable */

/* stack functions */
int push(stack_t *stack, int val, int pos);
int pop(stack_t *stack, int *val, int *pos);

/* program functions */
int getln(void);
int check_source(void);

/* program to check the source code for errors */
main()
{
	check_source();
	return 0;
}

/* push:  push values onto stack */
int push(stack_t *stack, int val, int pos)
{
	if (stack->top == MAXVAL) {
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
	if (stack->top == 0)
		return -1;
	stack->top--;
	*val = stack->val[stack->top];
	*pos = stack->pos[stack->top];
	return 0;
}

/* is_empty:  checks if stack is empty */
int is_empty(stack_t *stack)
{
	return (stack->top == 0);
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

/* check_source:  checks source code for syntax errors */
int check_source(void)
{
	extern char line[];
	stack_t stack_error;
	stack_t stack;
	state_t state;
	int comments;
	int quotes;
	int error;
	int c, d, i;
	int ln;

	comments = quotes = 0;
	error = ln = i = 0;
	while (getln() > 0) {
		++ln;
		switch (state) {
		case CODE:
			c = line[i++];
			if (c == '\'' || c == '"')
				state = QUOTES;
			else if (c == '/') {
					d = line[i];
				if (d == '*')
					state = COMMENT;
				else
					++i;
			} else if (c == '(' || c == '[' || c == '{') {
				push(&stack, c, ln);
			} else if (c == ')' || c == ']' || c == '}') {
				if (is_empty(&stack)) {
					printf("Syntax error line %d: '%c' found without"
						" counterpart.\n", ln, c);
					error = 1;
				} else {
					int val, pos;
					pop(&stack, &val, &pos);
					if ((val == '{' && c == '}') ||
						(val == '[' && c == ']') ||
						(val == ')' && c == ')')) {
						printf("Syntax error line %d: '%c' does not match '%c'.\n",
							pos, val, c);
						error = 1;
					}
				}
			}
		break;
		case QUOTES:
			if (c == '\\')
				++i;
			else if (c == '\'' || c == '"') {
				++quotes;
				state = CODE;
			}
		break;
		case COMMENT:
			if (c == '*') {
				d = line[i];
				if (d == '/') {
					++comments;
					state = CODE;
				} else
					++i;
			}
		break;
		}
	}

	if (comments > 0 || quotes > 0) {
		printf("Comments: %d\n"
			"Quotes: %d\n",
			comments, quotes);
	}
	if (state == COMMENT) {
		printf("Code ends inside of comment.\n");
		error = 1;
	} else if (state == QUOTES) {
		printf("Code ends inside of quotes.\n");
		error = 1;
	}
	if (error == 0) {
		printf("Code seems to be okay.\n");
	} else {
		printf("There were errors in the code.\n");
	}
	return 0;
}
