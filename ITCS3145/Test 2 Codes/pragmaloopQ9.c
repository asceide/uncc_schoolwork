/*
    A) No, the code is not correct.
    B) The problem is the variable j. The variable is not independent from the loop, which is demanding a certain order i.e. when i=0, j=7, i=1, j=9, i=2, j=11 etc. It 
    cannot execute in any order safely like it is right now.
    The fix is simple however, which is to declare j inside the loop and set it to 5 + 2 (which is the increment ) * (i+1) (which would be the total increments)
*/


//Updated Code
int i, A[MAX];
#   pragma omp parallel for shared(A), private(i)
    for(i=0; i<MAX; i++){
        int j=5+2*(i+1);
        A[i]=big(j);
    }