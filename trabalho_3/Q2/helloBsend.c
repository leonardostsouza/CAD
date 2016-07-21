#include <stdio.h>
//#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int partner, myRank, buffer, world_size, proc_rank;
    MPI_Status status;
    
    /////////////// INITIALIZATION ///////////////
    MPI_Init(NULL, NULL);
    // get world size
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // get proccess rank
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    ///////////// END INITIALIZATION /////////////

    printf("Proccess rank %d sends \"Hello\"!"
    	" There are %d proccesses running\n", 
    	proc_rank, world_size);

    
    if (proc_rank < world_size/2){
        partner = proc_rank + world_size/2;
        myRank = proc_rank;
        MPI_Send( &myRank, 1, MPI_INT, partner, 0, MPI_COMM_WORLD );
        MPI_Recv( &buffer, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
    }
    else {
        partner = proc_rank - world_size/2;
        myRank = proc_rank;

        MPI_Recv( &buffer, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        MPI_Send( &myRank, 1, MPI_INT, partner, 0, MPI_COMM_WORLD );
    }

    printf("Tarefa %d tem parceria com %d, onde %d e o taskid da tarefa"
    " e de seu parceiro\n", proc_rank, partner, buffer);

    MPI_Finalize();
}


