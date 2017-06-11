#include <stdio.h>

int main(void) {
	int number;
	int max = -10000;
	int counter = 0;

	do {
		scanf("%d",&number);
		if(number != -1)
			counter++;
		if(number > max)
			max = number;
	} while(number != -1);
	if(counter)
		printf("The largest number is %d\n",max);
	else
		printf("Are you kidding? You haven't entered any numbers!\n");
	return 0;
}
