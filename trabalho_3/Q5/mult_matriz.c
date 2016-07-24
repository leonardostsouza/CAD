#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 4
#define MALLOC


int main(int argc, char** argv) {
    int send_partner, recv_partner, rows, world_size, proc_rank;
    int num_rows, rest, offset, i, j, k;
    MPI_Status status;

    #ifdef MALLOC
        double *A, *B, *C;
    #else
        double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    #endif

    #ifdef MALLOC
        // Criar matrizes
        A = (double *) malloc((SIZE*SIZE)*sizeof(double));
        B = (double *) malloc((SIZE*SIZE)*sizeof(double));
        C = (double *) malloc((SIZE*SIZE)*sizeof(double));

    #endif
    
    //MPI_Request reqSendA[world_size], reqSendB[world_size], reqRecv[world_size];
    
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

        // Fill matrices
        for (i = 0; i < (SIZE*SIZE); i++){
            A[i] = 1;
            B[i] = 1;
        }


        // Distribute rows
        num_rows = SIZE/(world_size-1);
        rest = SIZE%(world_size-1);
        offset = 0;
        int off = 0;
        for (send_partner = 1; send_partner < world_size; send_partner++){
            rows = (send_partner <= rest) ? num_rows+1 : num_rows;
            printf("MASTER sending %d rows to process %d (%d to %d)\n", rows, send_partner, off, off+rows-1);
            MPI_Send(&offset, 1, MPI_INT, send_partner, 0, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, send_partner, 0, MPI_COMM_WORLD);
            MPI_Send(&A[offset], rows*SIZE, MPI_DOUBLE, send_partner, 0, MPI_COMM_WORLD);
            MPI_Send(&B, SIZE*SIZE, MPI_DOUBLE, send_partner, 0, MPI_COMM_WORLD);
            offset = offset + (rows*SIZE);
            off = off+1;
        }

        // Receive results
        for (recv_partner=1; recv_partner < world_size; recv_partner++) {
            MPI_Recv(&offset, 1, MPI_INT, recv_partner, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, recv_partner, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[offset], rows*SIZE, MPI_DOUBLE, recv_partner, 0, MPI_COMM_WORLD, &status);
            printf("Proccess %d has finnished\n",recv_partner);
        }
        printf("Multiplication DONE! Printing results:\n");

        // Print matrix C
        for (i = 0; i < (SIZE*SIZE); i++){
            if(!i%SIZE){
                printf("\n");
            }
            printf("%6.2f\t", C[i]);
        }
        printf("\n");
    }

    /////////////// WORKER ///////////////
    else {
        // Receive Matrices
        MPI_Recv(&offset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&A[offset], rows*SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&B, SIZE*SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

        // Compute A*B
        // for (k=0; k<SIZE; k++) {
        //     for (i=0; i<rows; i++) {
        //         C[i][k] = 0.0;
        //         for (j=0; j<SIZE; j++){
        //             C[i][k] += A[i][j] * B[j][k];
        //         }
        //     }
        // }

        for (i = 0; i < rows; i++) {
            for (j = 0; j < SIZE; j++) {
                double sum = 0.0;
                for (k = 0; k < SIZE; k++)
                    sum = sum + A[i * SIZE + k] * B[k * SIZE + j];
                C[i * SIZE + j] = sum;
            }
        }

        // Send results to MASTER
        MPI_Send(&offset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&C, rows*SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    #ifdef MALLOC
    	for (i = 0; i < SIZE; i++){
    		free(A[i]);
    		free(B[i]);
    		free(C[i]);
    	}
        free(A);
        free(B);
        free(C);
    #endif
    MPI_Finalize();
}


