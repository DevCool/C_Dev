#include <stdio.h>

int main(void) {
	int exp;
	int pow = 1;

        /* loop that goes up to 16 */
	for(exp = 0; exp < 16; exp++) {
		printf("2 to the power of %d is %d\n",exp,pow);
		pow *= 2;
	}
        /* return successful execution */
	return 0;
}
