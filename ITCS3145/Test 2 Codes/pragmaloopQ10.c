/*
*   This is a parallelized code of a serial code using OpenMP
*/
double  ave=0.0, A[MAX];   
int i;

#   pragma omp parallel for \
    default(none) reduction(+: ave) private(i) \
    shared(A)
    for (i=0;i< MAX; i++) {
       ave+ = A[i];
    }

ave= ave/MAX;