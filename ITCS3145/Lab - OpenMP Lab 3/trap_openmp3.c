/* File:    trap_openmp3.c
 * Purpose: Calculate definite integral using trapezoidal 
 *          rule.
 *
 * Input:   a, b, n
 * Output:  Estimate of integral from a to b of f(x)
 *          using n trapezoids.
 *
 * Compile: gcc -g -Wall -fopenmp -o trap_openmp3 trap_openmp3.c
 * Usage:   ./trap_openmp3  ${num_threads}
 *
 * Note:    The function f(x) is hardwired.
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double f(double x);    /* Function we're integrating */
double Trap(double a, double b, int n, double h);

int main(int argc, char* argv[]) {
   double  local_integral = 0, global_integral = 0;   /* Store result in integral   */
   double  a, b;       /* Left and right endpoints   */
   int     n;          /* Number of trapezoids       */
   double  h;          /* Height of trapezoids       */

   int thread_count = strtol(argv[1], NULL, 10); //QC added it
   printf("Enter a, b, and n\n");
   scanf("%lf", &a);
   scanf("%lf", &b);
   scanf("%d", &n);

   h = (b-a)/n;
#  pragma omp parallel num_threads(thread_count) \
      reduction(+: global_integral)   
   {
	   local_integral = Trap(a, b, n, h);
	   global_integral += local_integral;

		int my_rank = omp_get_thread_num();
		printf("my rank: %d, local_integral: %.2f\n",
				my_rank, local_integral);
   }
   
   printf("With n = %d trapezoids, our estimate ", n);
   printf("of the integral from %.2f to %.2f = %.2f\n",
      a, b, global_integral);

   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Estimate integral from a to b of f using trap rule and
 *              n trapezoids
 * Input args:  a, b, n, h
 * Return val:  Estimate of the integral 
 */
double Trap(double a, double b, int n, double h) {
	   double local_integral;
	   int k;
	   int my_rank = omp_get_thread_num();
	   int thread_count = omp_get_num_threads();

	   double local_n = n / thread_count;
	   double local_a = a + h * local_n * my_rank;
	   double local_b = local_a + h * local_n ;

	   local_integral = (f(local_a) + f(local_b))/2.0;
	   for (k = 1; k <= local_n-1; k++) {
		   local_integral += f(local_a+k*h);
	   }
	   local_integral = local_integral*h;

	return local_integral;
}  /* Trap */

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
