#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 100000

int main(int argc, char** argv) {
    int send_partner, recv_partner, values, world_size, proc_rank;
    int num_values, rest, offset, i, j, k;
    MPI_Status status;
    
    double A[SIZE], B[SIZE], sum, temp_sum;
    
    /////////////// INITIALIZATION ///////////////
    MPI_Init(NULL, NULL);
    // get world size
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // get proccess rank
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    ///////////// END INITIALIZATION /////////////


    /////////////// MASTER ///////////////
    if (proc_rank == 0){

        printf("Proccess rank %d is MASTER." 
        " There are %d proccesses running\n", 
        proc_rank, world_size);

        // Fill vectors
        for (i = 0; i < SIZE; i++){
            A[i] = 1;
            B[i] = 1;
        }

        sum = 0;

        // Distribute values
        num_values = SIZE/(world_size-1);
        rest = SIZE%(world_size-1);
        offset = 0;
        for (send_partner = 1; send_partner < world_size; send_partner++){
            values = (send_partner <= rest) ? num_values+1 : num_values;
            printf("MASTER sending %d values to process %d (%d to %d)\n", values, send_partner, offset, offset+values-1);
            MPI_Send(&offset, 1, MPI_INT, send_partner, 0, MPI_COMM_WORLD);
            MPI_Send(&values, 1, MPI_INT, send_partner, 0, MPI_COMM_WORLD);
            MPI_Send(&A[offset], values, MPI_DOUBLE, send_partner, 0, MPI_COMM_WORLD);
            MPI_Send(&B[offset], values, MPI_DOUBLE, send_partner, 0, MPI_COMM_WORLD);
            offset = offset + values;
        }

        // Receive results
        for (recv_partner=1; recv_partner < world_size; recv_partner++) {
            MPI_Recv(&offset, 1, MPI_INT, recv_partner, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&values, 1, MPI_INT, recv_partner, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&temp_sum, 1, MPI_DOUBLE, recv_partner, 0, MPI_COMM_WORLD, &status);
            printf("Proccess %d has finnished\n",recv_partner);
            sum += temp_sum;
        }

        printf("\nInner Product DONE! SUM = %6.2f\n", sum);
    }

    /////////////// WORKER ///////////////
    else {
        // Receive Matrices
        MPI_Recv(&offset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&values, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&A, values, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&B, values, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

        // Compute AxB
        temp_sum = 0;
        for (i=0; i<values; i++) {
            temp_sum += A[i] * B[i];
        }

        // Send results to MASTER
        MPI_Send(&offset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&values, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&temp_sum, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    } 

    MPI_Finalize();
}