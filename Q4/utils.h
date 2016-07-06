#include <stdint.h>

#define LEN  199998 //1048576

// return number of cicles since start
uint64_t rdtsc(){
    unsigned int lo,hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

// execute quicksort
void quicksort(int numbers[], int left, int right)
{
    int i, j, aux, pivot = left;

    for(i = left+1; i <= right ; i++)
    {
        j = i;

        aux = numbers[j];
        if(numbers[j] < numbers[pivot])
        {
         while(j > pivot)
         {    
            numbers[j] = numbers[j-1];
            j--;
         }
         numbers[j] = aux;
         pivot++;        
        }  
    }

    #pragma omp parallel firstprivate(pivot)
    {

    #pragma omp sections 
        {
            #pragma omp section
            {
                if(pivot - 1 >= left)
                {
                    quicksort(numbers, left, pivot-1);
                }
            }

            #pragma omp section
            {
                if(pivot + 1 <= right)
                {
                    quicksort(numbers, pivot+1, right);
                }
            }
        }
    }
}