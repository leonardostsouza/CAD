#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define SIZE 4

int main(int argc, char** argv) {
    int send_partner, recv_partner, myRank, buffer, world_size, proc_rank, i, j;
    MPI_Status status;
    MPI_Request reqSendA[world_size], reqSendB[world_size], reqRecv[world_size];

    
    /////////////// INITIALIZATION ///////////////
    MPI_Init(NULL, NULL);
    // get world size
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // get proccess rank
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    ///////////// END INITIALIZATION /////////////

    if (proc_rank == 0){
        // Criar matrizes
        float *A = malloc(sizeof(float)*(SIZE));
        float *B = malloc(sizeof(float)*(SIZE));
        float *C = malloc(sizeof(float)*(SIZE));

        for (i = 0; i<SIZE; i++){
            A[i] = malloc(sizeof(float)*(SIZE));
            B[i] = malloc(sizeof(float)*(SIZE));
            C[i] = malloc(sizeof(float)*(SIZE));
        }

        for (i = 0; i<SIZE; i++){
            A[i] = 1;
            B[i] = 1;
        }
        // transpor B

        // distribuir
        for (i = 0; i < SIZE; i++){
            // enviar
            for(j = 0; j < SIZE; j++){
                MPI_Isend( &A[i], SIZE, MPI_FLOAT, j%world_size , 0, MPI_COMM_WORLD, &reqSendA[j]);
                MPI_Isend( &B[j], SIZE, MPI_FLOAT, j%world_size , 0, MPI_COMM_WORLD, &reqSendB[j]);
            }

            // receber
            for(j = 0; j < SIZE; j++){
                MPI_Irecv( &C[i][j], 1, MPI_FLOAT, j%world_size, 0, MPI_COMM_WORLD, &reqRecv[j]);
            }

            for(j = 0; j < SIZE; j++){
                MPI_Wait(&reqSendA[j], &status);
                MPI_Wait(&reqSendB[j], &status);
                MPI_Wait(&reqRecv[j], &status);
            }
        }
        // imprimir resultado
        for (i = 0; i < SIZE; i++){
            for (j = 0; j < SIZE; j++){
                printf("C[%d][%d] = %.f", i, j, C[i][j]);
            }
        }
    }
    else {
        // declarar buffers
        float *A = malloc(sizeof(float)*(SIZE));
        float *B = malloc(sizeof(float)*(SIZE));
        // receber matrizes

        for (i = 0; i < SIZE; i++){
            // enviar
            for(j = 0; j < SIZE; j++){
                MPI_Isend( &A[i], SIZE, MPI_FLOAT, j%world_size , 0, MPI_COMM_WORLD, &reqSendA[j]);
                MPI_Isend( &B[j], SIZE, MPI_FLOAT, j%world_size , 0, MPI_COMM_WORLD, &reqSendB[j]);
            }

            // receber
            for(j = 0; j < SIZE; j++){
                MPI_Irecv( &C[i][j], 1, MPI_FLOAT, j%world_size, 0, MPI_COMM_WORLD, &reqRecv[j]);
            }

            for(j = 0; j < SIZE; j++){
                MPI_Wait(&reqSendA[j], &status);
                MPI_Wait(&reqSendB[j], &status);
                MPI_Wait(&reqRecv[j], &status);
            }
        }
        // calcular produto
        // enviar resposta pro mestre
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
    free(C);
    MPI_Finalize();
}


