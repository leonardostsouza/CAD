default:
	echo "blocking | nblocking | run | clean"

blocking:
	mpicc -O -o run helloBsend.c

nblocking:
	mpicc -O -o run helloNBsend.c

run: 
	mpirun -n 8 run
	
clean:
	rm -v run