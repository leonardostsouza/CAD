#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 4
//#define MALLOC

void printMatrix(double** M, int size){
    int i, j;
    for (i = 0; i < size; i++){
        printf("\n");
        for (j = 0; j < size; j++){
            printf("%6.2f\t", M[i][j]);
        }
    }
}

int main(int argc, char** argv) {
    int send_partner, recv_partner, rows, world_size, proc_rank;
    int num_rows, rest, offset, i, j, k;
    MPI_Status status;

    #ifdef MALLOC
        double **A, **B, **C;
    #else
        double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    #endif
    
    //MPI_Request reqSendA[world_size], reqSendB[world_size], reqRecv[world_size];
    
    /////////////// INITIALIZATION ///////////////
    MPI_Init(NULL, NULL);
    // get world size
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // get proccess rank
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    ///////////// END INITIALIZATION /////////////

    #ifdef MALLOC
        // Criar matrizes
        A = (double**) malloc(sizeof(*A) * SIZE);
        B = (double**) malloc(sizeof(*B) * SIZE);
        C = (double**) malloc(sizeof(*C) * SIZE);

        for (i = 0; i<SIZE; i++){
            A[i] = (double*) malloc(sizeof(*A[i]) * SIZE);
            B[i] = (double*) malloc(sizeof(*A[i]) * SIZE);
            C[i] = (double*) malloc(sizeof(*A[i]) * SIZE);
        }
    #endif

    /////////////// MASTER ///////////////
    if (proc_rank == 0){

        printf("Proccess rank %d is MASTER." 
        " There are %d proccesses running\n", 
        proc_rank, world_size);

        // Fill matrices
        for (i = 0; i < SIZE; i++){
            for (j = 0; j < SIZE; j++) {
                A[i][j] = 1;
                B[i][j] = 1;
            }
        }


        // Distribute rows
        num_rows = SIZE/(world_size-1);
        rest = SIZE%(world_size-1);
        offset = 0;
        for (send_partner = 1; send_partner < world_size; send_partner++){
            rows = (send_partner <= rest) ? num_rows+1 : num_rows;
            printf("MASTER sending %d rows to process %d (%d to %d)\n", rows, send_partner, offset, offset+rows-1);
            MPI_Send(&offset, 1, MPI_INT, send_partner, 0, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, send_partner, 0, MPI_COMM_WORLD);
            MPI_Send(&A[offset][0], rows*SIZE, MPI_DOUBLE, send_partner, 0, MPI_COMM_WORLD);
            MPI_Send(&B, SIZE*SIZE, MPI_DOUBLE, send_partner, 0, MPI_COMM_WORLD);
            offset = offset + rows;
        }

        // Receive results
        for (recv_partner=1; recv_partner < world_size; recv_partner++) {
            MPI_Recv(&offset, 1, MPI_INT, recv_partner, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, recv_partner, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&C[offset][0], rows*SIZE, MPI_DOUBLE, recv_partner, 0, MPI_COMM_WORLD, &status);
            printf("Proccess %d has finnished\n",recv_partner);
        }
        printf("Multiplication DONE! Printing results:\n");

        // Print matrix C
        for (i = 0; i < SIZE; i++){
            printf("\n");
            for (j = 0; j < SIZE; j++){
                printf("%6.2f\t", C[i][j]);
            }
        }
        printf("\n");
    }

    /////////////// WORKER ///////////////
    else {
        // Receive Matrices
        MPI_Recv(&offset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&A, rows*SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&B, SIZE*SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

        // Compute A*B
        for (k=0; k<SIZE; k++) {
            for (i=0; i<rows; i++) {
                C[i][k] = 0.0;
                for (j=0; j<SIZE; j++){
                    C[i][k] += A[i][j] * B[j][k];
                }
            }
        }

        // Send results to MASTER
        MPI_Send(&offset, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&C, rows*SIZE, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    #ifdef MALLOC
        free(A);
        free(B);
        free(C);
    #endif
    MPI_Finalize();
}


