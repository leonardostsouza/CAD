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

    int partner;
    int buffer;
    MPI_Status status;
    if (proc_rank < world_size/2){
        partner = world_size/2 + proc_rank;
        MPI_Send( &proc_rank, sizeof(int), MPI_INT, partner, 0, MPI_COMM_WORLD );
        MPI_Recv( &buffer, sizeof(int), MPI_INT, 0, partner, MPI_COMM_WORLD, &status );
    }
    else{
        partner = proc_rank - world_size/2;
        MPI_Recv( &buffer, sizeof(int), MPI_INT, 0, partner, MPI_COMM_WORLD, &status );
        MPI_Send( &proc_rank, sizeof(int), MPI_INT, partner, 0, MPI_COMM_WORLD );
    }

    

    printf("Tarefa %d tem parceria com %d, onde %d e o taskid da tarefa"
    " e de seu parceiro\n", proc_rank, partner, buffer);


    MPI_Finalize();
}