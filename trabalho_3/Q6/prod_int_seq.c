#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 100000

int main(int argc, char** argv) {
    int i;
    double A[SIZE], B[SIZE], sum;

    // Fill vectors
    for (i = 0; i < SIZE; i++){
        A[i] = 1;
        B[i] = 1;
    }

    sum = 0;
    for (i=0; i<SIZE; i++){
        sum = sum + (A[i] * B[i]);
    }

    printf("\nInner Product DONE! SUM = %6.2f\n", sum);

}