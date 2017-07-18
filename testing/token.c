#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

typedef enum PARSE_TOKENS {
  TOKEN_NONE = -1,
  TOKEN_SLASH,
  TOKEN_ASTERISK,
  TOKEN_POUND,
  TOKEN_EQUAL,
  TOKEN_COUNT
} parse_tokens_t;

char tokens[TOKEN_COUNT] = {
  '/', '*', '#', '='
};

#define ERROR(M) fprintf(stderr, M);

void die(const char *format, ...) {
  va_list args;
  va_start(args, format);
  vfprintf(stderr, format, args);
  va_end(args);
  exit(1);
}
#define DIE(M) { die(M); goto error; }

void parse(char *line);
void procfile(FILE *fp);

int main(int argc, char *argv[]) {
  FILE *fp = NULL;

  if((fp = fopen("mytest.b", "rt")) == NULL)
    DIE("[Info] : Could not open mytest.b\n");
  procfile(fp);
  if(fp != NULL)
    fclose(fp);
  return 0;

 error:
  if(fp != NULL)
    fclose(fp);
  exit(1);
}

void parse(char *line) {
  char *s = line;
  int tok_found;
  int c;

  tok_found = 0;
  while(*s != 0) {
    switch(*s) {
    case '/':
      c = TOKEN_SLASH;
      ++tok_found;
      break;
    case '*':
      c = TOKEN_ASTERISK;
      ++tok_found;
      break;
    case '#':
      c = TOKEN_POUND;
      ++tok_found;
      break;
    case '=':
      c = TOKEN_EQUAL;
      ++tok_found;
      break;
    default:
      c = TOKEN_NONE;
    }
    putchar(tokens[c]);
    ++s;
  }
  putchar('\n');
}

void procfile(FILE *fp) {
  char line[128];

  while(fgets(line, sizeof line, fp) != NULL) {
    parse(line);
  }
  puts("Done processing file.");
}
