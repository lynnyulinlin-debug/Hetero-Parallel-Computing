// reduce_cuda_naive.cu —— CUDA 朴素归约。
// 每个线程累加自己的 strided 子序列，再用原子操作写回全局结果。
#include "reduce.h"
#include "../common/cuda_helpers.cuh"

__global__ void reduce_naive_kernel(const float *a, float *out, int n) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;
    float local = 0.0f;
    for (int i = tid; i < n; i += stride) {
        local += a[i];
    }
    atomicAdd(out, local);
}

extern "C" void reduce_cuda_naive(const float *a, float *sum, int n) {
    float *d_a = nullptr;
    float *d_sum = nullptr;
    size_t bytes = (size_t)n * sizeof(float);

    CUDA_CHECK(cudaMalloc(&d_a, bytes));
    CUDA_CHECK(cudaMalloc(&d_sum, sizeof(float)));
    CUDA_CHECK(cudaMemcpy(d_a, a, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemset(d_sum, 0, sizeof(float)));

    int threads = 256;
    int blocks = (n + threads - 1) / threads;
    if (blocks > 1024) blocks = 1024;
    if (blocks < 1) blocks = 1;
    reduce_naive_kernel<<<blocks, threads>>>(d_a, d_sum, n);
    CUDA_CHECK_KERNEL();

    CUDA_CHECK(cudaMemcpy(sum, d_sum, sizeof(float), cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaFree(d_a));
    CUDA_CHECK(cudaFree(d_sum));
}
