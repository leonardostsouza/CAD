default:
	gcc -O2 -fopenmp quicksort.c -o qs

tau:
	tau_cc.sh -O2 -openmp quicksort.c -o qs

clean:
	rm -v qs
	rm -v profile.*
	rm -v monte_carlo.pomp.c
	rm -v *.o
