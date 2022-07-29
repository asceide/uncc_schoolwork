/* File:    trap_openmp4.c
 * Purpose: Calculate definite integral using trapezoidal 
 *          rule.
 *
 * Input:   a, b, n
 * Output:  Estimate of integral from a to b of f(x)
 *          using n trapezoids.
 *
 * Compile: gcc -g -Wall -fopenmp -o trap_openmp2 trap_openmp2.c
 * Usage:   ./trap_openmp4  ${num_threads}
 *
 * Note:    The function f(x) is hardwired.
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double f(double x);    /* Function we're integrating */


int main(int argc, char* argv[]) {
   double  local_integral = 0, global_integral = 0;   /* Store result in integral   */
   double  a, b;       /* Left and right endpoints   */
   int     n;          /* Number of trapezoids       */
   double  h;          /* Height of trapezoids       */
double approx=0;
   int thread_count = strtol(argv[1], NULL, 10); //QC added it
   printf("Enter a, b, and n\n");
   scanf("%lf", &a);
   scanf("%lf", &b);
   scanf("%d", &n);

   h = (b-a)/n;
   approx=(f(a)+f(b))/2.0;
#  pragma omp parallel for num_threads(thread_count) \
      reduction(+: approx)   
       for(int i=1; i<=n-1; i++)
           approx+=f(a+i*h);
       
       approx=h*approx;
/*		printf("my rank: %d, local_integral: %.2f\n",
				my_rank, local_integral);*/
   
   
   printf("With n = %d trapezoids, our estimate ", n);
   printf("of the integral from %.2f to %.2f = %.2f\n",
      a, b, approx);

   return 0;
}  /* main */


/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input args:  x
 */
double f(double x) {
   double return_val;

   return_val = x*x;
   return return_val;
}  /* f */
