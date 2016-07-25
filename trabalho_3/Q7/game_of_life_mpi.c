/***********************
Jogo da Vida
Versão Paralela
************************/
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define NI 8        /* tamanho dos array  */
#define NJ 8

#define NSTEPS 100    /* Numero de iteracoes */
#define SEED 1000

int main(int argc, char *argv[]) 
{

  int i, j, n, im, ip, jm, jp, ni, nj, 
  nsum, isum, proc_rank, world_size;
  int **old, **new;  
  float x;
  MPI_Status status;

  srand48(SEED);

  /////////////// INITIALIZATION ///////////////
  MPI_Init(NULL, NULL);
    // get world size
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // get proccess rank
  MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    ///////////// END INITIALIZATION /////////////

  printf("Proccess rank %d starting...\n", proc_rank);

  /* decomposição de dominiio */
  ni = (NI/world_size)+2;  /* celulas fantasmas na borda  */
  nj = (NJ/world_size)+2;

  ni = proc_rank <= NI%(world_size) ? ni+1 : ni;
  nj = proc_rank <= NJ%(world_size) ? nj+1 : nj;

  /* alocacao */
  
  old = malloc(ni*sizeof(int*));
  new = malloc(ni*sizeof(int*));

  for(i=0; i<ni; i++){
    old[i] = malloc(nj*sizeof(int));
    new[i] = malloc(nj*sizeof(int));
  }

/*  inicializando elementos  */

  for(i=1; i<=NI; i++)
  {
    for(j=1; j<=NJ; j++)
    {
      x = drand48();
      if(x<0.5){
        old[i][j] = 0;
      } else {
        old[i][j] = 1;
      }
    }
  }

  /* */
  for(n=0; n<NSTEPS; n++){  
    if (proc_rank == 0){
      // se primeiro processo
      MPI_Send(&old[NI-1][0], NJ, MPI_INT, 1, 0, MPI_COMM_WORLD);
      MPI_Recv(&old[NI][0], NJ, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
    } else if (proc_rank == (world_size-1)) {
      // se ultimo processo
      MPI_Recv(&old[0][0], NJ, MPI_INT, proc_rank-1, 0, MPI_COMM_WORLD, &status);
      MPI_Send(&old[1][0], NJ, MPI_INT, proc_rank-1, 0, MPI_COMM_WORLD);
    } else {
      // processo padrão
      MPI_Recv(&old[0][0], NJ, MPI_INT, proc_rank-1, 0, MPI_COMM_WORLD, &status);
      MPI_Send(&old[1][0], NJ, MPI_INT, proc_rank-1, 0, MPI_COMM_WORLD);
      MPI_Send(&old[NI-1][0], NJ, MPI_INT, proc_rank+1, 0, MPI_COMM_WORLD);
      MPI_Recv(&old[NI][0], NJ, MPI_INT, proc_rank+1, 0, MPI_COMM_WORLD, &status);
    }

    /* condicoes de controno para as esquinas do dominio */
    old[0][0]       = old[ni-2][NJ];
    old[0][NJ+1]    = old[ni-2][1];
    old[ni-1][NJ+1] = old[1][1];
    old[ni-1][0]    = old[1][NJ];

    /* cond. contorno para faces direita/esquerda  */

    for(i=1; i<=(ni-2); i++){
      old[i][0]    = old[i][NJ];
      old[i][NJ+1] = old[i][1];
    }

    /* cond. controno face inferior e superior */

    for(j=1; j<=NJ; j++){
      old[0][j]    = old[ni-2][j];
      old[ni-1][j] = old[1][j];
    }

    // Codigo Paralelo: Trocar elementos da interface paralela

    for(i=1; i<=(ni-2); i++)
    {
     for(j=1; j<=NJ; j++)
     {
       im = i-1;
       ip = i+1;
       jm = j-1;
       jp = j+1;

       nsum =  old[im][jp] + old[i][jp] + old[ip][jp]
       + old[im][j ]              + old[ip][j ] 
       + old[im][jm] + old[i][jm] + old[ip][jm];

       switch(nsum)
       {
         case 3:
         new[i][j] = 1;
         break;
         case 2:
         new[i][j] = old[i][j];
         break;
         default:
         new[i][j] = 0;
       }
     }
   }

    /* copia estado  */
   for(i=1; i<=(ni-2); i++){
    for(j=1; j<=NJ; j++){
     old[i][j] = new[i][j];
   }
 }
}

  /*  Conta o número de celulas  vivas no final */
isum = 0;
for(i=1; i<=(ni-2); i++){
  for(j=1; j<=NJ; j++){
    isum = isum + new[i][j];
  }
}
printf("\n# Celulas Vivas = %d\n", isum);


for(i=0; i<ni; i++){
  free(old[i]); 
  free(new[i]); 
}

free(old);
free(new);

MPI_Finalize();

return 0;
}
