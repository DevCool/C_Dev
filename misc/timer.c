#include <stdio.h>
#include <time.h>

int main(void) {
    clock_t start = clock();
    
    while(1) {
        double secondsPassed = (clock() - start) / CLOCKS_PER_SEC;
        printf("Time passed: %f\n", secondsPassed);
        if(secondsPassed >= 5)
            break;
    }
    return 0;
}
