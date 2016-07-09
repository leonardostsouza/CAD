
#include "monte_carlo.c.opari.inc"
#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char* argv[])
{
	double pi;
	unsigned int qtt_threads;
	unsigned long N, M;

	// check parameter
	if (argc != 3)
	{
		printf("Usage: \"mcpi <N> <K>\" , where N is the # of random points to be generated and K is the number of threads\n");
	}
	else
	{
		// initialize variables
		N = strtoul(argv[1], NULL, 10);//atoi(argv[1]);
		qtt_threads = atoi(argv[2]);
		M = 0;
		srand48(0);//rdtsc());

{
  int pomp2_num_threads = qtt_threads;
  int pomp2_if = 1;
  POMP2_Task_handle pomp2_old_task;
  POMP2_Parallel_fork(&pomp2_region_1, pomp2_if, pomp2_num_threads, &pomp2_old_task, pomp2_ctc_1 );
		#pragma omp parallel firstprivate(N) reduction(+:M)                          POMP2_DLIST_00001 firstprivate(pomp2_old_task) if(pomp2_if) num_threads(pomp2_num_threads)
{   POMP2_Parallel_begin( &pomp2_region_1 );
		{
			unsigned int itt = 0;
			unsigned int tempM = 0;
{   POMP2_For_enter( &pomp2_region_2, pomp2_ctc_2  );
			#pragma omp for nowait
			for(itt = 0; itt < N; itt++)
			{
				// generate point
				double x = drand48() - 0.5;
				double y = drand48() - 0.5;

				// check if point is inside circle
				double z = (x*x) + (y*y);
				if (z <= RADIUS_2)
				{
					tempM++;
				}
			}
{ POMP2_Task_handle pomp2_old_task;
  POMP2_Implicit_barrier_enter( &pomp2_region_2, &pomp2_old_task );
#pragma omp barrier
  POMP2_Implicit_barrier_exit( &pomp2_region_2, pomp2_old_task ); }
  POMP2_For_exit( &pomp2_region_2 );
 }
			M += tempM;
		}
{ POMP2_Task_handle pomp2_old_task;
  POMP2_Implicit_barrier_enter( &pomp2_region_1, &pomp2_old_task );
#pragma omp barrier
  POMP2_Implicit_barrier_exit( &pomp2_region_1, pomp2_old_task ); }
  POMP2_Parallel_end( &pomp2_region_1 ); }
  POMP2_Parallel_join( &pomp2_region_1, pomp2_old_task ); }

		// calculate PI
		pi = (M/(double)N) * 4;

		printf("N = %lu, M = %lu, PI = %.5f\n", N, M, pi);		
	}
	return 0;
}
