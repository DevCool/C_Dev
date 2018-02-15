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
void push(stack_t *stack, int val, int pos);
int pop(stack_t *stack, int *val, int *pos);

/* program functions */
int getln(void);
int check_source(void);

/* program to check the source code for errors */
main()
{
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

/* getln:  get line of input from stdin */
int getln(void)
{
	extern char line[];
	int c, i;
	for (i = 0; i < MAXLINE-2 && (c = getchar()) != EOF && c != '\n'; i++)
		line[i] = c;
	if (c == '\n')
		line[i++] = '\n'
	line[i] = '\0';
	return i;
}

/* check_source:  checks source code for syntax errors */
int check_source(void)
{
	stack_t stack_error;
	stack_t stack;
	state_t state;
	return 0;
}
