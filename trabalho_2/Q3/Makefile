default:
	gcc -O2 -fopenmp monte_carlo.c -o mcpi -lm 

tau:
	tau_cc.sh -O2 -openmp monte_carlo.c -o mcpi -lm 

clean:
	rm -v mcpi
	rm -v profile.*
	rm -v monte_carlo.pomp.c
	rm -v *.o