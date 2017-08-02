#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static char alpha[] = {
  'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
  'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
  'x', 'y', 'z'
};
static char nums[] = {
  '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'
};

int main(void) {
  char buf[9];
  unsigned int i = 1337; /* 7+6+12+8 = 33 dec */
  int j, k;
  
  i ^= 1000 << 1 & 0xff;
  srand(i);
  memset(buf, 0, sizeof buf);
  printf("%d\n", i);
  for(j = 0; j < sizeof(buf); j++) {
    k = rand()%2;
    switch(k) {
    case 0:
      buf[j] = alpha[rand()%26];
      break;
    case 1:
      buf[j] = nums[rand()%10];
      break;
    default:
      break;
    }
  }
  buf[j] = 0;
  printf("Password : [%8s]\n", buf);
  return 0;
}
