#include <stdio.h>
#include <stdlib.h>

#define SIZE 100000

int main(int argc, char** argv) {
    int i;
    double A[SIZE], B[SIZE], sum = 0;

    // Fill vectors
    for (i = 0; i < SIZE; i++){
        A[i] = 1;
        B[i] = 2;
    }

    printf("%.f, %.f\n", A[10], B[15]);

    for (i=0; i < SIZE; i++){
        sum = sum + (A[i] * B[i]);
    }

    printf("\nInner Product DONE! SUM = %6.2f\n", sum);

    return 0;
}