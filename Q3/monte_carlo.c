#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char* argv[])
{
	double x, y, z, pi;
	unsigned int itt, niter, countM;

	//printf("rdtsc: %lu\n",(unsigned long)rdtsc());

	// check parameter
	if (argc != 2)
	{
		printf("Usage: \"mcpi <N>\" , where N is the # of random points to be generated\n");
	}
	else
	{
		// initialize variables
		niter = atoi(argv[1]);
		countM = 0;
		srand48(rdtsc());

		#pragma omp parallel for shared(niter) private(x, y, itt) reduction(+:countM) num_threads(8)
		for(itt = 0; itt < niter; itt++)
		{
			// generate point
			x = drand48() - 0.5;
			y = drand48() - 0.5;

			// check if point is inside circle
			if (((x*x) + (y*y)) <= (RADIUS*RADIUS))//isInsideCircle(x, y) == 1)
			{
				countM++;
			}
		}

		// calculate PI
		pi = (countM/(double)niter) * 4;

		printf("N = %u, M = %u, PI = %.5f\n", niter, countM, pi);		
	}
	return 0;
}