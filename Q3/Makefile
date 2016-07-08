default:
	gcc -O -fopenmp monte_carlo.c -o mcpi

tau:
	tau_cc.sh -O2 -openmp monte_carlo.c -o mcpi

clean:
	rm -v mcpi
	rm -v profile.*
	rm -v monte_carlo.pomp.c
	rm -v *.o