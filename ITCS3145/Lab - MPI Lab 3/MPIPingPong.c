#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(){
    const int MAX_PINGPONG=10;
    int my_rank;
    int comm_size;
    int count=0;
    int pong=0;
    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    if(comm_size%2!=0){
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if(my_rank%2==0){
        pong=my_rank+1;
    }
    else
    {
        pong=my_rank-1;
    }
    
    while(count<MAX_PINGPONG){
        if(my_rank%2==count%2){
            count++;
            MPI_Send(&count, 1, MPI_INT, pong, 0, MPI_COMM_WORLD);
            printf("\nProcess %d sent ping %d to %d", my_rank, count, pong);
        }else{
            MPI_Recv(&count, 1, MPI_INT, pong, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("\nProcess %d received pong %d from %d", my_rank, count, pong);
        }
    }
     
    MPI_Finalize();
}