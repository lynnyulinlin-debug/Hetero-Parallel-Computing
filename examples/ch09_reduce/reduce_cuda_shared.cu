// reduce_cuda_shared.cu —— CUDA 共享内存归约。
// 先在 block 内做树形归约，再把每个 block 的部分和写回全局结果。
#include "reduce.h"
#include "../common/cuda_helpers.cuh"

__global__ void reduce_shared_kernel(const float *a, float *out, int n) {
    extern __shared__ float sdata[];
    unsigned int tid = threadIdx.x;
    unsigned int base = blockIdx.x * (blockDim.x * 2);
    float local = 0.0f;

    if (base + tid < n) {
        local += a[base + tid];
    }
    if (base + tid + blockDim.x < n) {
        local += a[base + tid + blockDim.x];
    }

    sdata[tid] = local;
    __syncthreads();

    for (unsigned int s = blockDim.x / 2; s > 32; s >>= 1) {
        if (tid < s) {
            sdata[tid] += sdata[tid + s];
        }
        __syncthreads();
    }

    if (tid < 32) {
        volatile float *v = sdata;
        if (blockDim.x >= 64) v[tid] += v[tid + 32];
        if (blockDim.x >= 32) v[tid] += v[tid + 16];
        if (blockDim.x >= 16) v[tid] += v[tid + 8];
        if (blockDim.x >= 8)  v[tid] += v[tid + 4];
        if (blockDim.x >= 4)  v[tid] += v[tid + 2];
        if (blockDim.x >= 2)  v[tid] += v[tid + 1];
    }

    if (tid == 0) {
        atomicAdd(out, sdata[0]);
    }
}

extern "C" void reduce_cuda_shared(const float *a, float *sum, int n) {
    float *d_a = nullptr;
    float *d_sum = nullptr;
    size_t bytes = (size_t)n * sizeof(float);

    CUDA_CHECK(cudaMalloc(&d_a, bytes));
    CUDA_CHECK(cudaMalloc(&d_sum, sizeof(float)));
    CUDA_CHECK(cudaMemcpy(d_a, a, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemset(d_sum, 0, sizeof(float)));

    int threads = 256;
    int blocks = (n + threads * 2 - 1) / (threads * 2);
    if (blocks > 1024) blocks = 1024;
    if (blocks < 1) blocks = 1;
    reduce_shared_kernel<<<blocks, threads, threads * sizeof(float)>>>(d_a, d_sum, n);
    CUDA_CHECK_KERNEL();

    CUDA_CHECK(cudaMemcpy(sum, d_sum, sizeof(float), cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaFree(d_a));
    CUDA_CHECK(cudaFree(d_sum));
}
