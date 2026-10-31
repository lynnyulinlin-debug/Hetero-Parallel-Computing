// gemm_cuda.cu —— GEMM 的 CUDA 分块版。
// 目标：演示计算受限任务如何通过 tile + shared memory 提高数据复用。
#include <stddef.h>

#include "gemm.h"
#include "../common/cuda_helpers.cuh"

static constexpr int TILE = 16;

__global__ void gemm_kernel(const float *a, const float *b, float *c, int n) {
    __shared__ float As[TILE][TILE];
    __shared__ float Bs[TILE][TILE];

    int row = blockIdx.y * TILE + threadIdx.y;
    int col = blockIdx.x * TILE + threadIdx.x;
    float sum = 0.0f;

    for (int t = 0; t < (n + TILE - 1) / TILE; ++t) {
        int a_col = t * TILE + threadIdx.x;
        int b_row = t * TILE + threadIdx.y;

        As[threadIdx.y][threadIdx.x] =
            (row < n && a_col < n) ? a[(size_t)row * (size_t)n + (size_t)a_col] : 0.0f;
        Bs[threadIdx.y][threadIdx.x] =
            (b_row < n && col < n) ? b[(size_t)b_row * (size_t)n + (size_t)col] : 0.0f;

        __syncthreads();

        #pragma unroll
        for (int k = 0; k < TILE; ++k) {
            sum += As[threadIdx.y][k] * Bs[k][threadIdx.x];
        }

        __syncthreads();
    }

    if (row < n && col < n) {
        c[(size_t)row * (size_t)n + (size_t)col] = sum;
    }
}

extern "C" void gemm_cuda(const float *a, const float *b, float *c, int n) {
    size_t bytes = (size_t)n * (size_t)n * sizeof(float);
    float *d_a = nullptr;
    float *d_b = nullptr;
    float *d_c = nullptr;

    CUDA_CHECK(cudaMalloc(&d_a, bytes));
    CUDA_CHECK(cudaMalloc(&d_b, bytes));
    CUDA_CHECK(cudaMalloc(&d_c, bytes));
    CUDA_CHECK(cudaMemcpy(d_a, a, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_b, b, bytes, cudaMemcpyHostToDevice));

    dim3 threads(TILE, TILE);
    dim3 blocks((n + TILE - 1) / TILE, (n + TILE - 1) / TILE);
    gemm_kernel<<<blocks, threads>>>(d_a, d_b, d_c, n);
    CUDA_CHECK_KERNEL();

    CUDA_CHECK(cudaMemcpy(c, d_c, bytes, cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaFree(d_a));
    CUDA_CHECK(cudaFree(d_b));
    CUDA_CHECK(cudaFree(d_c));
}
