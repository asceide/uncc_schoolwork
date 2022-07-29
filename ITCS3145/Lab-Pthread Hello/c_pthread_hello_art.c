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

void* dance();//Thread function that prints 'dance'
void* music();//Thread function that prints 'music'

int main(int argc, char* argv[]){
	long thread;//holds the thread number
	pthread_t* dance_thread;//array of threads for the dance function
    pthread_t* music_thread;//array of threads for the music function

	thread_count = strtol(argv[1], NULL, 10);//gets the number of threads of the CML
	dance_thread = malloc(thread_count * sizeof(pthread_t));//creates a variable array of threads to be used for the dance threads
    music_thread = malloc(thread_count*sizeof(pthread_t));//creates a variable array of threads to be used for the music threads

	//Thread creation, since no parameters are needed in the functions, pass NULL as the 4th arg
	for (thread = 0; thread < thread_count; thread++){
		pthread_create(&dance_thread[thread], NULL, dance, NULL);
        pthread_create(&music_thread[thread], NULL, music, NULL);
	}

	//Joins the threads
	for (thread = 0; thread < thread_count; thread++){
		pthread_join(dance_thread[thread], NULL);
        pthread_join(music_thread[thread], NULL);
	}
	//Frees the thread array
 	free(dance_thread);
    free(music_thread);
	//Points the variables to null
	dance_thread = NULL;
    music_thread = NULL;

	return 0;
}

//Pthread function that prints Dance
void* dance(){
    printf("\nDance");
    return NULL;
}
//Pthread function that prints Music
void* music(){
    printf("\nMusic");
    return NULL;
}