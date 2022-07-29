#define N 10000000

void vector_add(float *out, float *a, float *b, int n) {
    int i
#   pragma omp parallel for private(i) \
    shared(n, out, a, b)
    for(i = 0; i < n; i ++){
        out[i] = a[i] + b[i];
    }
}

int main(){
    float *a, *b, *out; 
    int i;
    // Allocate memory
    a   = (float*)malloc(sizeof(float) * N);
    b   = (float*)malloc(sizeof(float) * N);
    out = (float*)malloc(sizeof(float) * N);

    // Initialize array
#   pragma omp parallel for private(i)
    for(i= 0; i < N; i++){
        a[i] = 1.0f;
        b[i] = 2.0f;
    }

    vector_add(out,a,b,N);


    // Deallocate host memory
    free(a); 
    free(b); 
    free(out);
}
