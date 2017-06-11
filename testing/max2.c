/* finding the larger of two numbers */
#include <stdio.h>

int main(void) {
	/* two numbers */
	int number1, number2;
	int max;

	/* read two numbers */
	scanf("%d",&number1);
	scanf("%d",&number2);

	/* temporarily assume that former number given is larger */
	max = number1;

	if(number2 > max)
		max = number2;

	/* print the result */
	printf("The largest number is %d\n",max);

	return 0;
}
