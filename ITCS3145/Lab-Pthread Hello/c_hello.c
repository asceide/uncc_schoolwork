/*
 * c_pthread_hello.c
 *
 *  Created on: Aug 29, 2019
 *      Author: qcheng1
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;

void * hello(void * rank);

int main(int argc, char* argv[]){
	long thread;
	pthread_t* thread_handlers;

	thread_count = strtol(argv[1], NULL, 10);
	thread_handlers = malloc(thread_count * sizeof(pthread_t));

	printf("Create pthread_t data structure");
	for (thread = 0; thread < thread_count; thread++){
		pthread_create(&thread_handlers[thread], NULL, hello, (void *) thread);
	}

	printf("Hello from main thread\n");

	for (thread = 0; thread < thread_count; thread++){
		pthread_join(thread_handlers[thread], NULL);
	}

 	free(thread_handlers);
	thread_handlers = NULL;

	return 0;
}

void* hello(void* rank){
	long my_rank = (long)rank;

	printf("Hello from thread %ld of %d\n", my_rank, thread_count);

	return NULL;
}
