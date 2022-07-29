/*
 * c_pthread_hello.c
 *
 *  Created on: Aug 29, 2019
 *      Author: qcheng1
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;//Global Var for the # of threads

void* dance(void* cost);//Dance thread function
void* music(void* cost);//Music thread function

int main(int argc, char* argv[]){
	int dance_cost=0;//var to hold the return value from the dance function
    int music_cost=0;//same as above, but for music
    long thread=0;//holds the thread number
	pthread_t* dance_thread;//array of pthreads meant for the dance function
    pthread_t* music_thread;//array of pthreads meant for the music funcion
    void* retVal=NULL;//var that will hold the return value from the pthread functions
	thread_count = strtol(argv[1], NULL, 10);//get the num of threads from the CML
	dance_thread = malloc(thread_count * sizeof(pthread_t));//creates an array of dance threads
    music_thread = malloc(thread_count*sizeof(pthread_t));//creates an array of music threads
    //Creates and initializes pthreads for their various functions, sends the address of the ints as the 4th arg to their respective function
	for (thread = 0; thread < thread_count; thread++){
		pthread_create(&dance_thread[thread], NULL, dance, &dance_cost);
        pthread_create(&music_thread[thread], NULL, music, &music_cost);
	}

    //Joins the threads back to the master thread. Since the 2nd arg retval isn't null, the exit status/value of the thread is copied into the location pointed to by retval
    //In this case, retVal is the address of the parameter passed to the function
	for (thread = 0; thread < thread_count; thread++){
		pthread_join(dance_thread[thread], &retVal);
        pthread_join(music_thread[thread], &retVal);
	}
    //Prints the cost, frees the memory allocated to the thread arrays and sets the variables to null
    printf("\nThe cost of dancing: %d",dance_cost);
    printf("\nThe cost of music: %d",music_cost);
 	free(dance_thread);
    free(music_thread);
	dance_thread = NULL;
    music_thread = NULL;

	return 0;
}

/*
*Thread functions. The address to the integer variables in the main is passed as a parameter. Then the price set within the function is
*set to X, and then it is assigned to cost (and there therefore int variable cost) as an int (via (*(int*), which a point to a cast of int) 
 and then the void* is returned
*/
void* dance(void* cost){
    int price=5000;
    printf("\nDance");
    *((int*)cost)=price;
    return cost;
}

void* music(void* cost){
    int price=3000;
    printf("\nMusic");
    *((int*)cost)=price;
    return cost;
}