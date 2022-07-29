#define N 10000000

void vector_add(float *out, float *a, float *b, int n) {
    for(int i = 0; i < n; i++){
        out[i] = a[i] + b[i];
    }
}

int main(){
    float *a, *b, *out, *local_a, *local_b, *local_out; 
    int comm_size, rank;
    int array_per_thread;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Allocate memory
    if(rank==0){
        a   = (float*)malloc(sizeof(float) * N);
        b   = (float*)malloc(sizeof(float) * N);
        out = (float*)malloc(sizeof(float) * N);
        //Initialize the array
        for(int i = 0; i < N; i++){
            a[i] = 1.0f;
            b[i] = 2.0f;
        }
        array_per_thread=N/comm_size;
    }
    MPI_Bcast(&array_per_thread, 1, MPI_Int, 0, MPI_COMM_WORLD);
    //Allocate local arrays, no need to initialize them since they'll get arrays from the main thread
    local_a = (float*)malloc(sizeof(float)*array_per_thread);
    local_b = (float*)malloc(sizeof(float)*array_per_thread);
    local_out = (float*)malloc(sizeof(float)*array_per_thread);
    MPI_Scatter(a, array_per_thread, MPI_FLOAT, local_a, array_per_thread, MPI_FLOAT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, array_per_thread, MPI_FLOAT, local_b, array_per_thread, MPI_FLOAT, 0, MPI_COMM_WORLD);
    vector_add(local_out, local_a, local_b, array_per_thread);
    MPI_Gather(local_out, array_per_thread, MPI_FLOAT, out, array_per_thread, MPI_FLOAT, 0, MPI_COMM_WORLD);


    // Deallocate host memory
    free(a); 
    free(b); 
    free(out);
    free(local_a);
    free(local_b);
    free(local_out);
    MPI_Finalize();
}
