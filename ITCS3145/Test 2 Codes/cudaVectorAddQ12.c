#define N 10000000

__global__ void vector_add(float *out, float *a, float *b, int n) {
    int index = threadIdx.x;
    int stride = blockDim.x;

    for(int i = index; i < n; i += stride){
        out[i] = a[i] + b[i];
    }
}

int main(){
    float *a, *b, *out; 
    float *gpu_a, *gpu_b, *gpu_out;
    // Allocate memory
    a   = (float*)malloc(sizeof(float) * N);
    b   = (float*)malloc(sizeof(float) * N);
    out = (float*)malloc(sizeof(float) * N);

    // Initialize array
    for(int i = 0; i < N; i++){
        a[i] = 1.0f;
        b[i] = 2.0f;
    }
    //This will allocate the arrays in the GPU memory
    cudaMalloc((void**)&gpu_a, sizeof(float)*N);
    cudaMalloc((void**)&gpu_b, sizeof(float)*N);
    cudaMalloc((void**)&gpu_out, sizeof(float)*N);

    //Transfer data from the memory in the normal system to the GPU
    cudaMemcpy(gpu_a, a, sizeof(float) * N, cudaMemcpyHostToDevice);
    cudaMemcpy(gpu_b, b, sizeof(float) * N, cudaMemcpyHostToDevice);   
    
    //Execute the function in the gpu
    vector_add<<<1,10>>>(gpu_out, gpu_a, gpu_b, N);

    //Put the data from the GPU back to the main system
    cudaMemcpy(out, gpu_out, sizeof(float) * N, cudaMemcpyDeviceToHost);


    // Deallocate device memory
    cudaFree(gpu_a);
    cudaFree(gpu_b);
    cudaFree(gpu_out);

    // Deallocate host memory
    free(a); 
    free(b); 
    free(out);
}
