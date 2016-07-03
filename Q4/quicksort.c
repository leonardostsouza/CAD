#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(){
    int numbers[LEN], i;
    srand48(999999999);
    
    for(i=0; i<LEN; i++)
    {
        numbers[i] = lrand48();
    }
        
    quicksort(numbers, 0, LEN-1);
    
    for(i=0; i<LEN; i++)
        printf("%d, ",numbers[i]);    
    printf("\n");
    return 0;
}