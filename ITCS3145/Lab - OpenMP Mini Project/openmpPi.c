/*
*   Compile: gcc -g -Wall -fopenmp -o openmpPi openmpPi.c -lm
    Execute: ./openmpPi <# of threads>
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

const int MAX_THREADS=64;

void Usage();
void* computingPiPara(void* rank);

int main(int argc, char* argv[]) {
    long thread;
    int thread_count;
    double sum=0.0;
    double factor=0.0;
    long n;
    int i;
    /*Get the number of threads of the CML*/
    if(argc!=2){//Checking for errors for the command line input
        Usage();//Sends an error if the arg array does not equal 0 (program name + num of threads)
    }
    thread_count = strtol(argv[1], NULL, 10);//otherwise, sends the num of threads from the CML to the thread count, using base 10/decimal format.
    if(thread_count<=0 || thread_count>=MAX_THREADS){//If the number of threads passed as an arg is less than or equal to zero, or more than 64, we send an error and exit
        Usage();
    }
    printf("How many digits of pi do you want? ");//Asks the user for the number of digits in pi they want.
    scanf("%ld", &n);
    n=pow(10,n);
#   pragma omp parallel for num_threads(thread_count) \
        default(none) reduction(+: sum) private(i, factor) \
        shared(n)
    for(i=0; i<n; i++){
        if(i%2==0)
            factor=1.0;
        else
            factor=-1.0;
        sum+=4*factor/(2*i+1);
    }
    printf("Here is the sum: %.16f", sum);
    return EXIT_SUCCESS;
}
void Usage(){
    fprintf(stderr, "The number of threads has to be between 0 and %d . Goodbye. ", MAX_THREADS);
    exit(0);
}
