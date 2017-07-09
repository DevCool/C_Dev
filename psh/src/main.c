#include <stdio.h>

/* define my psh shell */
extern void psh_loop(void);

int main(void) {
  psh_loop();
  return 0;
}
