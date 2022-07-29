#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include "./my_rand.h"

int main(){
    int rnum=0, sum=0, pnum=0, my_rank, comm_size, next, prev;
    unsigned randSeed;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    unsigned seed=my_rank+1;
    randSeed=my_rand(&seed);
    randSeed=my_rand(&randSeed);
    rnum=(my_drand(&randSeed)*100)+1;
    next=(my_rank+1)%comm_size;
    if(my_rank==0){
        prev=comm_size-1;
    }
    else{
        prev=my_rank-1;
    }

    if(my_rank==0){
            MPI_Send(&rnum, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
            printf("Process %d is sending %d to Process %d\n", my_rank, rnum, next);
    }
    else{
        MPI_Recv(&pnum, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process %d got number %d from Process %d\n", my_rank, pnum, prev);
        sum=rnum+pnum;
        if(my_rank!=comm_size-1){
            MPI_Send(&sum, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
            printf("%d got random number %d and sent sum %d to Process %d\n", my_rank, rnum, sum, next);
        }else{
            printf("%d got random number %d and calculates the sum to be %d\n", my_rank, rnum, sum);
        }
    }
    MPI_Finalize();

}