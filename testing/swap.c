#include <stdio.h>

int main(void) {
	int nums[5] = {8,10,6,4,2};
	int i, swapped;

	do {
		swapped = 0;
		for(i = 0; i < 4; i++)
			if(i < 4)
				if(nums[i] > nums[i+1]) {
					int tmp;
					swapped = 1;
					tmp = nums[i];
					nums[i] = nums[i+1];
					nums[i+1] = tmp;
				}
	} while(swapped);
	printf("List of numbers:\n");
	for(i = 0; i < 5; i++)
		printf("\t%d: %d\n", i, nums[i]);
	return 0;
}
