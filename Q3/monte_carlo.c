#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main(int argc, char* argv[])
{
	double x, y, z, pi;
	unsigned int itt, qtt_iter, qtt_threads, countM;

	//printf("rdtsc: %lu\n",(unsigned long)rdtsc());

	// check parameter
	if (argc != 3)
	{
		printf("Usage: \"mcpi <N> <M>\" , where N is the # of random points to be generated and M is the number of threads\n");
	}
	else
	{
		// initialize variables
		qtt_iter = atoi(argv[1]);
		qtt_threads = atoi(argv[2]);
		countM = 0;
		srand48(rdtsc());

		#pragma omp parallel for shared(qtt_iter) private(x, y, itt) reduction(+:countM) num_threads(qtt_threads)
		for(itt = 0; itt < qtt_iter; itt++)
		{
			// generate point
			x = drand48() - 0.5;
			y = drand48() - 0.5;

			// check if point is inside circle
			if (((x*x) + (y*y)) <= (RADIUS*RADIUS))
			{
				countM++;
			}
		}

		// calculate PI
		pi = (countM/(double)qtt_iter) * 4;

		printf("N = %u, M = %u, PI = %.5f\n", qtt_iter, countM, pi);		
	}
	return 0;
}