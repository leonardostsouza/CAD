#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char* argv[])
{
	double x, y, z, pi;
	unsigned int itt, N, M, qtt_threads;

	//printf("rdtsc: %lu\n",(unsigned long)rdtsc());

	// check parameter
	if (argc != 3)
	{
		printf("Usage: \"mcpi <N> <M>\" , where N is the # of random points to be generated and M is the number of threads\n");
	}
	else
	{
		// initialize variables
		N = atoi(argv[1]);
		qtt_threads = atoi(argv[2]);
		M = 0;
		srand48(rdtsc());

		#pragma omp parallel for shared(N) private(x, y, itt) reduction(+:M) num_threads(qtt_threads)
		for(itt = 0; itt < N; itt++)
		{
			// generate point
			x = drand48() - 0.5;
			y = drand48() - 0.5;

			// check if point is inside circle
			z = (x*x) + (y*y);
			if (z <= RADIUS_2)
			{
				M++;
			}
		}

		// calculate PI
		pi = (M/(double)N) * 4;

		printf("N = %u, M = %u, PI = %.5f\n", N, M, pi);		
	}
	return 0;
}