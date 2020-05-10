#include <mpi.h>
 #include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

int INF = 100000;
int size; //nombre de processeurs
int rank; //numéro du processeur
int* matrixBit; //morceau de la matrice accessible
int V; //nbsommets
int* displs; // displs specifies the displacement 
int* sendcounts; // sendcounts  specifying the number of elements to send to each processor
FILE *f_matrix;
int minWeight;
int* MST;
typedef struct { int p1; int p2; } Edge;


int main(int argc, char* argv[]){
clock_t start1,start2, end;
start1 = clock();

MPI_Init( &argc, &argv );

MPI_Comm_rank ( MPI_COMM_WORLD, &rank);
MPI_Comm_size ( MPI_COMM_WORLD, &size );
printf("rank is %d and number of processors is %d\n", rank , size);

if (rank == 0)
  {
  /*
  f_matrix = fopen("GraphMatrix.txt", "r");
  fscanf(f_matrix, "%d\n", &V);
	*/
  V = 1000; 
  printf("Number of vertices is %d\n", V);
  
  }

MPI_Bcast(&V, 1, MPI_INT, 0, MPI_COMM_WORLD);

int i,j,k;       

displs = (int*)malloc(sizeof(int) * size);   
sendcounts = (int*)malloc(sizeof(int) * size);

  displs[0] = 0;
  sendcounts[0] = V / size;
  int reste = size - (V % size); // Si le nb de processeurs ne divise pas V
  for (i = 1; i < reste; i++) 
  {
    sendcounts[i] = sendcounts[0];
    displs[i] = displs[i - 1] + sendcounts[i - 1];
  }
  for (i = reste; i < size; i++)
  {
    sendcounts[i] = sendcounts[0] + 1;
    displs[i] = displs[i - 1] + sendcounts[i - 1];
  }

int* graphMtrx;
  if (rank == 0)
  {
    graphMtrx = (int*)malloc(V*V*sizeof(int));
    /*
    for(i = 0; i<V; i++){
      for(j = 0; j<V-1; j++){
        int buf;
        fscanf(f_matrix,"%d", &buf);
        graphMtrx[V*i+j] = buf;
        if (buf != INF)
        printf("%d -- %d poids : %d\n", i ,j , graphMtrx[V*i+j] );
      }
      for(j = V-1; j<V; j++){
        int bin;
        fscanf(f_matrix,"%d/n", &bin);
        graphMtrx[V*i+j] = bin;
        if (bin != INF)
        printf("%d -- %d poids : %d\n", i ,j , graphMtrx[V*i+j] );
      }
    }
    fclose(f_matrix);
  */
  //random graph generator
  for (i = 0; i<V;i++){
    for (j = i+1; j<V;j++){
      int randomstuff = 1+rand()%10000;
      graphMtrx[i*V+j] = randomstuff;
      graphMtrx[j*V+i] = randomstuff;
    }
  }
  
  
  }
  

matrixBit = (int*)malloc(sendcounts[rank]*V*sizeof(int)); //pour chaque processeur on alloue sa partie
MPI_Datatype matrixString; 
MPI_Type_contiguous(V,MPI_INT, &matrixString);
MPI_Type_commit(&matrixString);
MPI_Scatterv(graphMtrx,sendcounts,displs,matrixString,matrixBit,sendcounts[rank],matrixString,0,MPI_COMM_WORLD);
/*
if(rank==0)
  {
    for (i = 0; i < V; ++i)
    {  
      for ( j = 0; j < V; ++j)
      {
        printf("%d ", matrixBit[V*i+j]); // just for test
      }
      printf("\n");
    }
  }
*/


start2 = clock();



MST = (int*)malloc(sizeof(int)*V);
for ( i = 0; i < V; i++) MST[i] = -1;
MST[0] = 0;
minWeight = 0;

////////////////////////////

int mini;
int p1 = 0;
int p2 = 0;

  struct { int mini; int rank; } pair, row;
  Edge edge;
  for ( k = 0; k < V - 1; k++)
  {
    mini = INF;
    for ( i = 0; i < sendcounts[rank]; i++)
    {
      if (MST[i + displs[rank]] != -1) 
      {
        for ( j = 0; j < V; j++)
        {
          if (MST[j] == -1) 
          {
            if ( matrixBit[V*i+j] < mini)
            {
              mini = matrixBit[V*i+j];
              p2 = j; // change the current edge
              p1 = i;   
            }
          }
        }
      }
    }
    row.mini = mini;
    row.rank = rank; // the rank of min in row
    MPI_Allreduce(&row, &pair, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD); //renvoie automatiquement l'arrête de poids minimum 
    edge.p1 = p1 + displs[rank];
    edge.p2 = p2;
    MPI_Bcast(&edge, 1, MPI_2INT, pair.rank, MPI_COMM_WORLD);

    MST[edge.p2] = edge.p1;
    minWeight += pair.mini;
  }
end = clock();

double cpu_time_used1,cpu_time_used2;
cpu_time_used1 = ((double) (end - start1)) / (double) CLOCKS_PER_SEC;
cpu_time_used2 = ((double) (end - start2)) / (double) CLOCKS_PER_SEC;


  if (rank == 0)
  {
  printf("\nBUILDING MST\n");
    for ( i=1; i< V; i++)
      printf("%d -- %d poids = %d\n",i, MST[i],graphMtrx[V*i+MST[i]]);      
    printf("\nNombre de processeurs: %d\nNombre de sommets: %d\nPoids Total: %d\n\n", size, V-1 , minWeight);
    printf("Temps total depuis le depart = %f secondes\n" ,cpu_time_used1);
    printf("Temps de calcul du MST = %f secondes\n" ,cpu_time_used2);
  
  }

MPI_Finalize();

return 0;
}