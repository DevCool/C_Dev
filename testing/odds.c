/* program to count even/odd numbers */
#include <stdio.h>

int main(void) {
	/* even/odd count variables */
	int even = 0, odd = 0;
	int num;

	/* display prompt to user */
	puts("You need to enter numbers (to quit enter 0).");
	scanf("%d",&num);

	/* while loop */
	while(num != 0) {
		if(num % 2) /* check if number is odd */
			odd++;
		else
			even++;

		/* read next number */
		scanf("%d",&num);
	}

	/* print result */
	printf("Even numbers: %d\nOdd numbers : %d\n",
			even, odd);
	return 0;
}
