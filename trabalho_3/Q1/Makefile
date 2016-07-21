default:
	mpicc -O -o run mpi_hello_world.c

run: 
	mpirun -n 4 run

clean
	rm -v run