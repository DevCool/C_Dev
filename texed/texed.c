/* Include standard headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* DLIST */
#include "dlist.h"

/* defines */
#define MAX_LINE 80

/* function prototypes */
int get_line(char *s, size_t size);

/* main() - entry point for this text editor.
 */
int main(int argc, char *argv[]) {
  char buf[MAX_LINE];
  int buf_len;

  do {
    memset(buf, 0, sizeof(buf));
    printf("CMD >> ");
    buf_len = get_line(buf, MAX_LINE);
    if(strncmp(buf, "add", 3) == 0)
      puts("Not yet implemented!");
    else if(strncmp(buf, "del", 3) == 0)
      puts("Not yet implemented!");
    else if(strncmp(buf, "exit", 4) == 0)
      break;
    else
      puts("Command not found.");
  } while(buf_len > 0);
  
  return 0;
}

/* get_line() - gets a string from the standard input.
 */
int get_line(char *s, size_t size) {
  int c, i, j;

  j = 0;
  for(i = 0; (c = getchar()) != EOF && c != 0x0A; i++)
    if(!(i < 0) && i < size-2) {
      s[i] = c;
      ++j;
    }
  if(c == 0x0A) {
    s[i] = c;
    ++i;
    ++j;
  }
  s[j] = 0;
  ++j;
  return i;
}
