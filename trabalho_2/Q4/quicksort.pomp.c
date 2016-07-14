
#include "quicksort.c.opari.inc"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(){
    char numbers[LEN];
    int i;
    //srand48(rdtsc());
    srand48(1234567890123);
    
{
  int pomp2_num_threads = omp_get_max_threads();
  int pomp2_if = 1;
  POMP2_Task_handle pomp2_old_task;
  POMP2_Parallel_fork(&pomp2_region_1, pomp2_if, pomp2_num_threads, &pomp2_old_task, pomp2_ctc_1 );
    #pragma omp parallel     private(i) shared(numbers) POMP2_DLIST_00001 firstprivate(pomp2_old_task) if(pomp2_if) num_threads(pomp2_num_threads)
{   POMP2_Parallel_begin( &pomp2_region_1 );
{   POMP2_For_enter( &pomp2_region_1, pomp2_ctc_1  );
    #pragma omp          for                                              nowait
    for(i=0; i<LEN; i++)
    {
        numbers[i] = lrand48();
    }
{ POMP2_Task_handle pomp2_old_task;
  POMP2_Implicit_barrier_enter( &pomp2_region_1, &pomp2_old_task );
#pragma omp barrier
  POMP2_Implicit_barrier_exit( &pomp2_region_1, pomp2_old_task ); }
  POMP2_For_exit( &pomp2_region_1 );
 }
  POMP2_Parallel_end( &pomp2_region_1 ); }
  POMP2_Parallel_join( &pomp2_region_1, pomp2_old_task ); }
        
    quicksort(numbers, 0, LEN-1);
    
    #if 0
    for(i=0; i<LEN; i++)
        printf("%d, ",numbers[i]);    
    printf("\n");
    #endif

    return 0;
}
