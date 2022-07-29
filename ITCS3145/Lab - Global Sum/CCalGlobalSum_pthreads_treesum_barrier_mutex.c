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
#include <sys/time.h>
#include <unistd.h>
#include <math.h>

// How to compile it?
// gcc -g -Wall -o CCalGlobalSum_pthreads_treesum_barrier_semaphore CCalGlobalSum_pthreads_treesum_barrier_semaphore.c -lpthread -lm

long sum = 0;
long times = 0, block_size = 0;
int thread_count = 0;
int count=0;

pthread_mutex_t mutex;
pthread_mutex_t barrier_mutex;

long * thread_sum = NULL;
int* counts;


void* computingGlobalSum_pthreads(void * threadID);

int main(int argc, char * argv[]) {
	thread_count = strtol(argv[1], NULL, 10);
	times = strtol(argv[2], NULL, 10);

	if ((thread_count <= 0) || (times <= 0)){
		printf("Usage: ./computingPi_pthreads  #(threads) N_times\n");
		printf("#(threads) > 0 and N_times > 0\n");
		exit(1);
	}

	thread_sum = malloc(thread_count * sizeof(long));
	int levels = ceil(log(thread_count) / log(2)) + 1;
	counts = malloc(levels * sizeof(int));
	for (int i = 0; i < levels ; i++ )
		counts[i] = 0;

	struct timeval begin, end;
	gettimeofday(&begin, NULL);

	block_size = times % thread_count == 0 ? times/thread_count : times/thread_count + 1;
	// allocate a container of thread handlers
	pthread_t * thread_handlers = malloc(thread_count * sizeof(pthread_t));


	for (int thread = 0; thread < thread_count; thread++){
		long threadID = thread;
		if (pthread_create(&thread_handlers[thread], NULL, computingGlobalSum_pthreads, (void *)threadID)){
			printf("Error creating a thread (id: %d)\n", thread);
			exit(1);
		}
	}

	//printf("%d threads are working on their local calculation and synchronizing with master thread\n", thread_count);

	for (int thread = 0; thread < thread_count; thread++){
		if (pthread_join(thread_handlers[thread], NULL)){
			printf("Error joining a thread (id: %d)\n", thread);
			exit(1);
		}
	}
	gettimeofday(&end, NULL);

	long seconds = end.tv_sec - begin.tv_sec;
	long micros = (seconds * 1000000) + end.tv_usec - begin.tv_usec;

	printf("Thread_count=%d;N=%ld;Sum=%ld;Seconds=%ld;Micros=%ld\n", thread_count, times, thread_sum[0], seconds, micros);

	free(thread_handlers);
	free(counts);
	pthread_mutex_destroy(&barrier_mutex);
	pthread_mutex_destroy(&mutex);


	return EXIT_SUCCESS;
}

void* computingGlobalSum_pthreads(void * threadID){
	// Local operation
	int my_rank = (int)((long) threadID);

	thread_sum[my_rank] = 0;
	long my_first_id = block_size * my_rank;
	long my_last_id = block_size * (my_rank + 1) > times ? times : block_size * (my_rank + 1);

	for (long i = my_first_id; i < my_last_id; i++){
		thread_sum[my_rank] += i;
	}
	printf("Local sum of thread %d from %ld to %ld is equal to %ld\n", my_rank, my_first_id, my_last_id, thread_sum[my_rank]);

	// Cooperation when shared memory, sum
	int iLevel = 0;
	for (int stride = 1; stride < thread_count; stride *= 2){ //So stride is basically the index that this thread will add to in a tree structure. For example, in i=0, stride would be 1, so that means that the index 0 will sum with index 1, 1 with 2, 2 with 3 etc. Stride 2 would be index 0 with index 2, index 1 with index 3, etc.
		if (my_rank % (iLevel == 0 ? 1 : 2 * iLevel) == 0){ //Basically figures out which threads will be the ones active. When ilevel is 0 all the threads are active. When i level is 1 then cores 0,2,4,6,8 etc are active. ilvl 2, 0,4,8, ilvl 3, 0, 6, 12 (my_rank%(2*3)=myrank%6)
			int active_threads = iLevel == 0 ? thread_count : floor(thread_count / (2 * iLevel));
			pthread_mutex_lock(&barrier_mutex);
			counts[iLevel]++;
			pthread_mutex_unlock(&barrier_mutex);
			while(counts[iLevel]<active_threads);//at ilvl one, 8 threads. l2, 4t, l3, 2t
			if (my_rank % (2 * stride) == 0){
				pthread_mutex_lock(&mutex);
				thread_sum[my_rank] += thread_sum[my_rank + stride];
				pthread_mutex_unlock(&mutex);
			}
			printf("%d threads completed barrier %d \n", active_threads, iLevel);
		}

		iLevel++;
	}

	return NULL;
}
