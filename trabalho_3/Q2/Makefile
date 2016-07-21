default:
	mpicc -O -o run helloBsend.c

run: 
	mpirun -n 8 run

clean
	rm -v run