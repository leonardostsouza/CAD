#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char* argv[])
{
	double x, y, z, pi;
	unsigned int itt, niter, count;

	// check parameter
	if (argc != 2)
	{
		printf("Usage: \"mcpi <N>\" , where N is the # of random points to be generated\n");
	}
	else
	{
		// initialize variables
		niter = atoi(argv[1]);
		count = 0;
		srand48(rdtsc());

		#pragma omp parallel shared(count, niter) private(x, y, itt) num_threads(8)
		{
			#pragma omp for
			for(itt = 0; itt < niter; itt++)
			{
				// generate point
				x = drand48() - 0.5;
				y = drand48() - 0.5;

				// check if point is inside circle
				if (((x*x) + (y*y)) <= (RADIUS*RADIUS))//isInsideCircle(x, y) == 1)
				{
					count ++;
				}
		}
		}

		// calculate PI
		pi = (count/(double)niter) * 4;

		printf("N = %d, M = %d, PI = %.5f\n", niter, count, pi);		
	}
	return 0;
}