/* program two of boolean values */
#include <stdio.h>

int main(void) {
    /* initial variables i,j initialized */
    int i = 1, j = 0;
    int k;

    /* (0 + 1 + 0 + 0) = 1 */
    k = (i <= 0) + (j >= 0) + (i > 1) + (j < 0);
    printf("%d\n",k);
    return 0;
}
