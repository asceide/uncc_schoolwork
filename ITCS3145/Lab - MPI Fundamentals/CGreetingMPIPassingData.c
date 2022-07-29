/**
*   Simple MPI Program that launches N Processes and prints a message
**/
#include <stdio.h>
#include <string.h> //In order to use string length
#include <mpi.h>


const int MAX_STRING=100; //Max length of string via a char array

int main(void){
    char greeting[MAX_STRING];
    char greeting2[MAX_STRING];
    int comm_sz; //Num of processes
    int my_rank; //The process number

    MPI_Init(NULL,NULL); //Initilizes the MPI, can only be done by the thread that calls MPI_finalize, the main thread. Typically takes in argc,argv
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);//gets the number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Status status;
    //Step by step 2A
    if(my_rank!=0){
        sprintf(greeting, "First round process %d of %d > Have a nice Day!", my_rank, comm_sz);
        MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        printf("First round Process %d of %d > Job Done!\n", my_rank, comm_sz);
    }
    else{
        for(int i=1; i<comm_sz; i++){
            MPI_Recv(greeting, MAX_STRING, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status);
            printf("%d receives a message from %d: %s\n", my_rank, status.MPI_SOURCE, greeting);
        }

        
    }
    //Any one 2B
    if(my_rank==0){
        for(int i=1; i<comm_sz; i++){
            MPI_Recv(greeting, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
            printf("%d receives a message from %d: %s\n", my_rank, status.MPI_SOURCE, greeting);
        }
    }else{
        sprintf(greeting, "Second round process %d of %d > Have a nice Day!", my_rank, comm_sz);
        MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
        printf("Second round Process %d of %d > Job Done!\n", my_rank, comm_sz);

    }
    //With tags 2C
    if(my_rank==0){
        for(int i=1; i<comm_sz; i++){
            MPI_Recv(greeting2, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("%d receives a message from %d with tag %d: %s\n", my_rank, i, status.MPI_TAG, greeting2);
        }
    }
    else{
        sprintf(greeting2, "Third round process %d of %d > Have a nice Day!", my_rank, comm_sz);
        MPI_Send(greeting2, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
        printf("Third round Process %d of %d > Job Done!\n", my_rank, comm_sz);

    }

    MPI_Finalize();
    return 0;
}