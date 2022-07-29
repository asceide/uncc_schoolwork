#include <stdio.h>
#include <mpi.h>


int main(){
    int comm_size, my_rank, next, prev, rounds, pnum;
    int count=0;
    int sum=5;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    
    
    if(my_rank==0){
        printf("\nPlease enter the number of rounds that will be done: \n");
        scanf("%d", &rounds);
    }
    MPI_Bcast(&rounds, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    next=(my_rank+1)%comm_size;
    if(my_rank==0){
        prev=comm_size-1;
    }
    else{
        prev=my_rank-1;
    }
    while(count<rounds){
        if(my_rank==0){
            if(count==0){//If its the first round, I want the user input, or in this case the hard coded initial value, to be multiplied by 2 prior to sending it to the next process. Otherwise it should only multiply the previous number.
                sum=sum*2;
            }else
            {
                sum=pnum*2;
            }
            MPI_Send(&sum, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
            printf("\nProcess %d sent %d to Process %d\n", my_rank, sum, next);
            MPI_Recv(&pnum, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("\nProcess %d received number %d\n", my_rank, pnum);
            count++;
        }else{
            MPI_Recv(&pnum, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("\nProcess %d received %d from Process %d\n", my_rank, pnum, prev);
            sum=pnum*2;
            MPI_Send(&sum, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
            printf("\nProcess %d sent %d to Process %d\n", my_rank, sum, next);
            count++;
    }
    }
    MPI_Finalize();

}