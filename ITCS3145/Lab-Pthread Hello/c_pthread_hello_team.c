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
const int TEAM_MEMBERS=2;//# of team members

void * hello(void * rank);//pthread function to print hello

int main(int argc, char* argv[]){
	long thread;//variable to hold the thread number
	pthread_t* thread_handlers;//an array of pthread objects

	thread_count = strtol(argv[1], NULL, 10);//takes in the arguments from the CML to initialize the pthreads
	thread_handlers = malloc(thread_count * sizeof(pthread_t));//creates an variable length array of pthreads

	printf("Create pthread_t data structure");
	for (thread = 0; thread < thread_count; thread++){
		pthread_create(&thread_handlers[thread], NULL, hello, (void *) thread);
	}

	printf("CCI\n");//Prints out CCI from the main thread

	for (thread = 0; thread < thread_count; thread++){//Joins the Pthreads to the main thread, return of the pthreads is NULL
		pthread_join(thread_handlers[thread], NULL);
	}

 	free(thread_handlers);//Frees the thread_handlers array from the memory
	thread_handlers = NULL;//Sets it to NULL

	return 0;
}

void* hello(void* rank){//Thread function
	long my_rank = (long)rank;//takes the parameter and parses it to a long integer 

	if(my_rank%TEAM_MEMBERS==0){//If the thread is even it prints out my name
		printf("\nThread #%ld : Patrick Alarcon", my_rank);
	}else if(my_rank%TEAM_MEMBERS==1)//If the thread is odd it prints out daniels name.
	{
		printf("\nThread #%ld : Daniel Brady", my_rank);
	}
	

	return NULL;
}
