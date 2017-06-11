#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define MAX_WIDTH 80
#define MAX_HEIGHT 24
#define MAX_LENGTH 80

char *terminal_buffer;

void setup_termbuf(char *term_buf);
void destroy_termbuf(char *term_buf);
int getstr(char *buf, size_t size);

int main(int argc, char *argv[]) {
	char buf[MAX_LENGTH];
	int i;
	
/*	setup_termbuf(terminal_buffer); */
	while((i = getstr(buf, MAX_LENGTH)) > 0) {
		printf("You typed: %s", buf);
		if(strcmp(buf, "quit\n") == 0)
			break;
		memset(buf, 0, MAX_LENGTH);
	}
/*	destroy_termbuf(terminal_buffer); */
	return 0;
}

void setup_termbuf(char *term_buf) {
	if(term_buf == NULL)
		term_buf = malloc(sizeof(char)*MAX_HEIGHT*MAX_WIDTH);
	memset(term_buf, 0, MAX_HEIGHT*MAX_WIDTH);
}

void destroy_termbuf(char *term_buf) {
	if(term_buf != NULL)
		free(term_buf);
}

int getstr(char *buf, size_t size) {
	int c, i, j;

	j = 0;
	for(i = 0; (c = getchar()) != EOF && c != 0x0D; ++i)
		if(i < size-2) {
			*(buf+i) = c;
			++j;
		} else if(c == 0x08) {
			*(buf+i) = 0;
			--i;
			--j;
		}
	if(c == 0x0D) {
		*(buf+i) = c;
		++i;
		++j;
	}
	*(buf+j) = 0x00;
	++j;
	return i;
}
