#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    
    /////////////// INITIALIZATION ///////////////
    // get world size
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // get proccess rank
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    ///////////// END INITIALIZATION /////////////

    printf("Proccess rank %d sends \"Hello\"!"
    	" There are %d proccesses running\n", 
    	proc_rank, world_size);
    
    MPI_Finalize();
}