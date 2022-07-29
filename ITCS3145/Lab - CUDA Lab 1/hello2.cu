#include <stdio.h>
#include <cuda.h>
// __global__ keyword specifies a device kernel function
__global__ void cuda_hello() {

   printf("Hello from GPU blockIdx.x=%d blockIdx.y=%d blockIdx.z=%d threadIdx.x=%d threadIdx.y=%d threadIdx.z=%d\n", blockIdx.x, blockIdx.y, blockIdx.z, threadIdx.x, threadIdx.y, threadIdx.z);
}

 

int main() {
    printf("Hello World from CPU!\n");
    // Call a device function from the host: a kernel launch Which will print from the device
    dim3 grid(16,16);
    dim3 block(16,16);
    cuda_hello<<<grid, block>>>();

     // This call waits for all of the submitted GPU work to complete

     cudaDeviceSynchronize();

      return 0;

}
