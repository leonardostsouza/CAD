#include <stdio.h>
#include <stdlib.h>

#define SIZE 100
//#define MALLOC

int main(int argc, char** argv) {
    int i, j, k;

    #ifdef MALLOC
        double **A, **B, **C;
    #else
        double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
    #endif

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

    // Fill matrices
    for (i = 0; i < SIZE; i++){
        for (j = 0; j < SIZE; j++) {
            A[i][j] = 1;
            B[i][j] = 1;
        }
    }

    // Compute A*B
    for (i = 0; i < SIZE; i++){
    	for (j = 0; j < SIZE; j++){
    		C[i][j] = 0.0;
    		for (k = 0; k < SIZE; k++){
    			C[i][j] += A[i][k] * B[j][k];
    		}
    	}
    }

    // printf("Multiplication DONE! Printing results:\n");

    // //Print matrix C
    // for (i = 0; i < SIZE; i++){
    //     printf("\n");
    //     for (j = 0; j < SIZE; j++){
    //         printf("%6.2f\t", C[i][j]);
    //     }
    // }
    // printf("\n");

    #ifdef MALLOC
	    for (int i = 0; i < SIZE; i++){
	    	free(A[i]);
		    free(B[i]);
		    free(C[i]);
	    }
	    free(A);
	    free(B);
	    free(C);
    #endif
}