#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(){
    char numbers[LEN];
    int i;
    //srand48(rdtsc());
    srand48(1234567890123);
    
    #pragma omp parallel for private(i) shared(numbers)
    for(i=0; i<LEN; i++)
    {
        numbers[i] = lrand48();
    }
        
    quicksort(numbers, 0, LEN-1);
    
    #if 0
    for(i=0; i<LEN; i++)
        printf("%d, ",numbers[i]);    
    printf("\n");
    #endif

    return 0;
}