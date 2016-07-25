#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int partner, myRank, buffer, world_size, proc_rank;
    MPI_Status status;
    MPI_Request reqSend, reqRecv;
    
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

    myRank = proc_rank;
    if (proc_rank < world_size/2){
        partner = proc_rank + world_size/2;
    }
    else {
        partner = proc_rank - world_size/2;
    }

    MPI_Isend( &myRank, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, &reqSend);
    MPI_Irecv( &buffer, 1, MPI_INT, partner, 0, MPI_COMM_WORLD, &reqRecv);

    MPI_Wait(&reqSend, &status);
    MPI_Wait(&reqRecv, &status);

    printf("Tarefa %d tem parceria com %d, onde %d e o taskid da tarefa"
    " e de seu parceiro\n", proc_rank, partner, buffer);

    MPI_Finalize();
    return 0;
}


