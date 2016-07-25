#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {
    int send_partner, recv_partner, myRank, buffer, world_size, proc_rank, i;
    float B, A, sum = 0;
    MPI_Status status;
    MPI_Request reqSend, reqRecv;
    
    /////////////// INITIALIZATION ///////////////
    MPI_Init(NULL, NULL);
    // get world size
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // get proccess rank
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    ///////////// END INITIALIZATION /////////////

    
    ///////////// DEFINE PARTNER /////////////
    send_partner = proc_rank +1;
    recv_partner = proc_rank -1;
    if (proc_rank == 0){
        recv_partner = world_size - 1;
    }
    else if (proc_rank == (world_size-1)){
        send_partner = 0;
    }
    /////////// END DEFINE PARTNER ///////////

    A = proc_rank;
    for (i = 0; i<world_size; i++){
        MPI_Isend( &A, 1, MPI_FLOAT, send_partner, 0, MPI_COMM_WORLD, &reqSend);
        MPI_Irecv( &B, 1, MPI_FLOAT, recv_partner, 0, MPI_COMM_WORLD, &reqRecv);

        MPI_Wait(&reqSend, &status);
        MPI_Wait(&reqRecv, &status);

        sum += B;
        A = B;
    }

    printf(" %d -> %d -> %d. SOMA: %.f\n", recv_partner, proc_rank, send_partner, sum);

    MPI_Finalize();
    return 0;
}


