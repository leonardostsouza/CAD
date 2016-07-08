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

		#pragma omp parallel firstprivate(N) reduction(+:M) num_threads(qtt_threads)
		{
			unsigned int itt = 0;
			unsigned int tempM = 0;
			#pragma omp for
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
			M += tempM;
		}

		// calculate PI
		pi = (M/(double)N) * 4;

		printf("N = %lu, M = %lu, PI = %.5f\n", N, M, pi);		
	}
	return 0;
}