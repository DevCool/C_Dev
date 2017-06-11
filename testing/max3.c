/* finding the largest number */
#include <stdio.h>

#define MAXNUMS 2	/* Change to one lower than you want to be max */

int main(void) {
	/* input number, max number */
	int input, max;
	int i;

	/* display startup message */
	printf("Please enter %d number(s) to see results.\n", MAXNUMS+1);

	/* read number from stdin */
	i = 0;
	while(1) {
		if(i > MAXNUMS)
			break;
		scanf("%d",&input);
		if(input > max)
			max = input;
		++i;
	}

	printf("The maximum value entered was %d out of %d different values.\n",
			max, i);
	return 0;
}
