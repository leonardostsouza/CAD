#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 10000

int main(int argc, char** argv) {
    int send_partner, recv_partner, myRank, buffer, world_size, proc_rank;
    MPI_Status status;
    MPI_Request reqSend, reqRecv;
    
    /////////////// INITIALIZATION ///////////////
    MPI_Init(NULL, NULL);
    // get world size
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // get proccess rank
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    ///////////// END INITIALIZATION /////////////

    float *A = malloc(sizeof(float)*(SIZE));
    float *B = malloc(sizeof(float)*(SIZE));
    int i;
    for (i = 0; i<SIZE; i++){
        A[i] = proc_rank;
    }

    send_partner = proc_rank +1;
    recv_partner = proc_rank -1;
    if (proc_rank == 0){
        recv_partner = world_size - 1;
    }
    else if (proc_rank == (world_size-1)){
        send_partner = 0;
    }

    for (i = 0; i<SIZE; i++){
        MPI_Isend( &A[i], 1, MPI_FLOAT, send_partner, 0, MPI_COMM_WORLD, &reqSend);
        MPI_Irecv( &B[i], 1, MPI_FLOAT, recv_partner, 0, MPI_COMM_WORLD, &reqRecv);

        MPI_Wait(&reqSend, &status);
        MPI_Wait(&reqRecv, &status);
    }


    printf(" %d -> %d -> %d. Recebeu: %.f"
    " \n", recv_partner, proc_rank, send_partner, B[0]);

    free(A);
    free(B);
    MPI_Finalize();
}


