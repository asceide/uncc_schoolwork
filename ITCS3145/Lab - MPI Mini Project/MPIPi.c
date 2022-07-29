/*
*   This program is a MPI implementation of POSIX Pi program that we did earlier this semester. 
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>


int main(){
    int my_rank, comm_size, n;
    long long local_n, first_i, last_i;
    double sum=0.0;
    double my_sum=0.0;
    double factor=0.0;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);

    if(my_rank==0){
        printf("\nHow many digits of pi do you want? \n");
        scanf("%d", &n);
        
    }
    n=pow(10,n);//powers the number of digits by 10
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);//Broadcasts the number of pi to the other threads
    MPI_Barrier(MPI_COMM_WORLD);

    local_n=n/comm_size;
    first_i=local_n*my_rank;
    last_i=first_i+local_n;
    if(first_i%2==0){
        factor=1.0;
    }else{
        factor=-1.0;
    }
    for(int i=first_i; i<last_i; i++, factor=-factor){
        my_sum+=4*factor/(2*i+1);
    }
    printf("\nRank %d with sum %f\n", my_rank, my_sum);
    MPI_Reduce(&my_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);//sends all of the local sums back to the root process.
    if(my_rank==0){
        printf("\nHere is the sum: %.10f\n Note that the above number is accurate to the number of pi digits you asked for, but nonetheless displays atleast 10 digits.", sum);
    }

    MPI_Finalize();
}
