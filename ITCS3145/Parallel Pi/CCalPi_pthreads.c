#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

pthread_mutex_t mutex;
const int MAX_THREADS=64;
int thread_count;
double sum;
long n;

void Usage();
void* computingPiPara(void* rank);

int main(int argc, char* argv[]) {
    long thread;
    pthread_t* thread_handles;
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
    pthread_mutex_init(&mutex,NULL);
    thread_handles=malloc(thread_count* sizeof(pthread_t));//Create an array of pthread_t opaque objects(pthread_t being a data structure that , one for each thread.
    for(thread=0; thread<thread_count; thread++){//Creates the threads
        pthread_create(&thread_handles[thread], NULL, computingPiPara, (void*) thread);//sends the pthread_t object, a null () since we won't be using the pthread_attr_t, the pthread routine, and an argument (usually the thread number) to pthread_create
    }
    printf("\nThis is the main thread, testing.");
    for(thread=0; thread<thread_count; thread++){
    	pthread_join(thread_handles[thread], NULL);
    }
    free(thread_handles);
    thread_handles=NULL;
    printf("Here is the sum: %.16f", sum);
    return EXIT_SUCCESS;
}

void* computingPiPara(void* rank){
	long my_rank=(long) rank;
	double factor,my_sum=0.0;
	long long i; 
	long long my_n=n/thread_count;
	long long my_first_i=my_n*my_rank;
	long long my_last_i=my_first_i+my_n;

	if (my_first_i%2==0){
		factor=1.0;
	}
	else
		factor=-1.0;
	for(i=my_first_i; i<my_last_i; i++, factor=-factor){
		my_sum+=4*factor/(2*i+1);
	}
    pthread_mutex_lock(&mutex);
    sum+=my_sum;
    pthread_mutex_unlock(&mutex);

	return NULL;

}

void Usage(){
    fprintf(stderr, "The number of threads has to be between 0 and %d . Goodbye. ", MAX_THREADS);
    exit(0);
}
