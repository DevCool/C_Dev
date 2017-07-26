#include <stdio.h>
#include <string.h>
#include <time.h>

/* printlookup functions */
float square(float a) {
  return (float)(a*a);
}

float cube(float a) {
  return (float)(a*a*a);
}

void printlookup(float (*fun)(float)) {
  float x, fx;

  for(x = 0.0f; x <= 1.0f; x += 0.01f) {
    fx = fun(x);
    printf("%10.3f - %10.3f\n", x, fx);
  }
}

/* printlookup2 functions */
char* time2(time_t now) {
  static char timestr[10], *tstr = NULL;
  time(&now);
  memset(timestr, 0, sizeof(timestr));
  strftime(timestr, sizeof timestr, "%H:%M:%S", localtime(&now));
  tstr = timestr;
  return tstr;
}

void wait(int sec) {
  clock_t start = clock();
  while((clock()-start)/CLOCKS_PER_SEC <= sec);
}

void printtime(char* (*fun2)(time_t), void (*wait)(int), int sec) {
  time_t now;
  char* timestr = NULL;
  int i;

  for(i = 0; i <= 5; i++) {
    timestr = fun2(now);
    printf("Time is: %s\n", timestr);
    if(sec > 0)
      wait(sec);
    else
      wait(1);
  }
}

int main(void) {
  puts(" *** Squared Table ***");
  printlookup(square);
  puts(" *** Cubed Table ***");
  printlookup(cube);
  printtime(time2, wait, 0);
  return 0;
}
