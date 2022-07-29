/* File:    trap.c
 * Purpose: Calculate definite integral using trapezoidal 
 *          rule.
 *
 * Input:   a, b, n
 * Output:  Estimate of integral from a to b of f(x)
 *          using n trapezoids.
 *
 * Compile: gcc -g -Wall -fopenmp -o trap_openmp1 trap_openmp1.c
 * Usage:   ./trap_openmp1 <num threads>
 *
 * Note:    The function f(x) is hardwired.
 *
 * IPP:     Section 3.2.1 (pp. 94 and ff.) and 5.2 (p. 216)
 */

#include <stdio.h>
#include <omp.h>

double f(double x);    /* Function we're integrating */
void Trap(double a, double b, int n, double h, double* result);

int main(int argc, char* argv[]) {
   double  integral=0;   /* Store result in integral   */
   double  a, b;       /* Left and right endpoints   */
   int     n;          /* Number of trapezoids       */
   double  h;          /* Height of trapezoids       */
   int thread_count;

   thread_count=strtol(argv[1],NULL,10);
   printf("Enter a, b, and n\n");
   scanf("%lf", &a);
   scanf("%lf", &b);
   scanf("%d", &n);
   h = (b-a)/n;
#  pragma omp parallel num_threads(thread_count)
   Trap(a, b, n, h, &integral);

   
   printf("With n = %d trapezoids, our estimate ", n);
   printf("of the integral from %.2f to %.2f = %.2f\n",
      a, b, integral);

   return 0;
}  /* main */

/*------------------------------------------------------------------
 * Function:    Trap
 * Purpose:     Estimate integral from a to b of f using trap rule and
 *              n trapezoids
 * Input args:  a, b, n, h
 * Return val:  Estimate of the integral 
 */
void Trap(double a, double b, int n, double h, double* result) {
   double my_result, temp_result, local_a, local_b;
   double integral;
   int local_n;
   int my_rank=omp_get_thread_num();
   int thread_count=omp_get_num_threads();

   local_n=n/thread_count;
   local_a=a+my_rank*local_n*h;
   local_b=local_a+local_n*h;
   integral=(f(local_a)+f(local_b))/2;
   for(int i=1; i<=local_n-1; i++){
         temp_result=local_a+i*h;
         integral+=f(temp_result);
   }
   integral=integral*h;
#  pragma omp critical
   *result+=integral;

   
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
