/* File:     hello_omp.c
 *
 * Purpose:  An openmp implementation of hello world
 *
 * Compile:  gcc -g -Wall -fopenmp -o hello_omp hello_omp.c
 * Run:      ./hello_omp.c <thread number>
 * 
 * Input:    Thread number
 * Output:   A greeting message
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void Hello(void);  /* Thread function */

/*--------------------------------------------------------------------*/
int main(int argc, char* argv[]) {
   int thread_count = strtol(argv[1], NULL, 10);

#  pragma omp parallel num_threads(thread_count)
   Hello();

   return 0; 
}  /* main */

/*-------------------------------------------------------------------
 * Function:    Hello
 * Purpose:     A function that prints message
 */
void Hello(void) {
   int my_rank=omp_get_thread_num();
   int thread_count=omp_get_num_threads();

   printf("Hello from thread %d of %d\n", my_rank, thread_count);

}  /* Hello */
