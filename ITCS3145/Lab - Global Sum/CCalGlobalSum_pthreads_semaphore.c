/*
 ============================================================================
 Name        : CTest.c
 Author      : Q. C.
 Version     :
 Copyright   : uncc.cci.cs
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <unistd.h>

long sum = 0;
long times = 0, block_size = 0;
int thread_count = 0;
sem_t semaphores;

void* computingGlobalSum_pthreads(void * threadID);

int main(int argc, char * argv[]) {

	thread_count = strtol(argv[1], NULL, 10);
	times = strtol(argv[2], NULL, 10);

	if ((thread_count <= 0) || (times <= 0)){
		printf("Usage: ./computingPi_pthreads  #(threads) N_times\n");
		printf("#(threads) > 0 and N_times > 0\n");
		exit(1);
	}

	block_size = times % thread_count == 0 ? times/thread_count : times/thread_count + 1;
	// allocate a container of thread handlers
	pthread_t * thread_handlers = malloc(thread_count * sizeof(pthread_t));
	sem_init(&semaphores, 0, 1);
	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	for (int thread = 0; thread < thread_count; thread++){
		if (pthread_create(&thread_handlers[thread], NULL, computingGlobalSum_pthreads, (void *)thread)){
			printf("Error creating a thread (id: %ld)\n", thread);
			exit(1);
		}
	}

	printf("%d threads are working on their local calculation and synchronizing with master thread\n", thread_count);

	for (int thread = 0; thread < thread_count; thread++){
		if (pthread_join(thread_handlers[thread], NULL)){
			printf("Error joining a thread (id: %ld)\n", thread);
			exit(1);
		}
	}
	gettimeofday(&end, NULL);
	long seconds = end.tv_sec - begin.tv_sec;
	long micros = (seconds * 1000000) + end.tv_usec - begin.tv_usec;
	printf("Thread_count=%d;N=%ld;Sum=%ld;Seconds=%ld;Micros=%ld\n", thread_count, times, sum, seconds, micros);
    sem_destroy(&semaphores);
    semaphores=NULL;

	return EXIT_SUCCESS;
}

void* computingGlobalSum_pthreads(void * threadID){
	// Local operation
	int my_rank = (int) threadID;

	long local_sum = 0;
	long my_first_id = block_size * my_rank;
	long my_last_id = block_size * (my_rank + 1) > times ? times : block_size * (my_rank + 1);

	double factor;

	for (long i = my_first_id; i < my_last_id; i++){
		local_sum += i;
	}
	printf("Local sum of thread %ld from %ld to %ld is equal to %ld\n", my_rank, my_first_id, my_last_id, local_sum);

	// Cooperationg with shared memory, sum
	sem_wait(&semaphores);//Basically the only difference is that Semaphores, via counters, can state how many threads can access a shared resource before its locked, where as a mutex is strictly just 1
	sum+=local_sum;
	sem_post(&semaphores);
	return NULL;
}
