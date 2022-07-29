/**
*   Simple MPI Program that launches N Processes and prints a message
**/
#include <stdio.h>
#include <string.h> //In order to use string length
#include <mpi.h>


const int MAX_STRING=100; //Max length of string via a char array

int main(void){
    char greeting[MAX_STRING];
    int comm_sz; //Num of processes
    int my_rank; //The process number

    MPI_Init(NULL,NULL); //Initilizes the MPI, can only be done by the thread that calls MPI_finalize, the main thread. Typically takes in argc,argv
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);//gets the number of processes
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    if(my_rank!=0){
        sprintf(greeting, "Greetings from process %d of %d!", my_rank, comm_sz);
        MPI_Send(greeting, strlen(greeting)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    else{
        printf("Greetings from process %d of %d\n", my_rank, comm_sz);
        for(int i=1; i<comm_sz; i++){
            MPI_Recv(greeting, MAX_STRING, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%s\n", greeting);
        }
    }
    MPI_Finalize();
    return 0;
}