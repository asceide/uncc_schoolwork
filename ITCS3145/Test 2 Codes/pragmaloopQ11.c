/*
*   This is a parallellized code of a serial Odd/Even sort using OpenMP
*/

void Odd_even_sort(int  arr[]  /* in/out */, int  n /* in     */) {
   int phase, i, temp;
   int sorted = 0;
#   pragma omp parallel for collapse(2) \ //This is assuming we can use collapse with our version of OpenMP
    default(none) private(phase, i, temp) \
    shared(arr, n, sorted)
   for (phase = 0; phase < n; phase++) {
   sorted = 1;
      if (phase % 2 == 0) { /* Even phase */
         for (i = 1; i < n; i += 2)
            if (arr[i-1] > arr[i]) {
               temp = arr[i];
               arr[i] = arr[i-1];
               arr[i-1] = temp;
               sorted = 0;
            }
      } else { /* Odd phase */
         for (i = 1; i < n-1; i += 2)
            if (arr[i] > arr[i+1]) {
               temp = arr[i];
               arr[i] = arr[i+1];
               arr[i+1] = temp;
               sorted = 0;
            }
      }
      if (sorted) break;
   }
}  /* Odd_even_sort */


////The Below Code is if the collapse parameter isn't able to be used with the used version of OpenMP
/*
 void Odd_even_sort(int  arr[], int  n  {
   int phase, i, temp;
   int sorted = 0;
#   pragma omp parallel \ //This is assuming we can use collapse with our version of OpenMP
    default(none) private(phase, i, temp) \
    shared(arr, n, sorted)
#   pragma omp for 
   for (phase = 0; phase < n; phase++) {
   sorted = 1;
      if (phase % 2 == 0) { //Even
         for (i = 1; i < n; i += 2)
            if (arr[i-1] > arr[i]) {
               temp = arr[i];
               arr[i] = arr[i-1];
               arr[i-1] = temp;
               sorted = 0;
            }
      } else { //ODD
#   pragma omp for 
         for (i = 1; i < n-1; i += 2)
            if (arr[i] > arr[i+1]) {
               temp = arr[i];
               arr[i] = arr[i+1];
               arr[i+1] = temp;
               sorted = 0;
            }
      }
      if (sorted) break;
   }
}  

*/