// softmax_cuda.cu —— 融合版 row-wise softmax。
// 目标：把载入、max 归约、exp/sum、归一化放进一个 kernel，减少中间张量往返。
#include <float.h>
#include <math.h>
#include <stddef.h>

#include "softmax.h"
#include "../common/cuda_helpers.cuh"

__global__ void softmax_kernel(const float *x, float *y, int rows, int cols) {
    extern __shared__ float smem[];
    float *row = smem;
    float *tmp = row + cols;

    int r = blockIdx.x;
    if (r >= rows) return;

    int tid = threadIdx.x;
    int base = r * cols;

    for (int c = tid; c < cols; c += blockDim.x) {
        row[c] = x[base + c];
    }
    __syncthreads();

    float local_max = -FLT_MAX;
    for (int c = tid; c < cols; c += blockDim.x) {
        local_max = fmaxf(local_max, row[c]);
    }
    tmp[tid] = local_max;
    __syncthreads();
    for (int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) tmp[tid] = fmaxf(tmp[tid], tmp[tid + s]);
        __syncthreads();
    }
    float maxv = tmp[0];

    float local_sum = 0.0f;
    for (int c = tid; c < cols; c += blockDim.x) {
        float e = expf(row[c] - maxv);
        row[c] = e;
        local_sum += e;
    }
    tmp[tid] = local_sum;
    __syncthreads();
    for (int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) tmp[tid] += tmp[tid + s];
        __syncthreads();
    }
    float sumv = tmp[0];
    float inv = 1.0f / sumv;

    for (int c = tid; c < cols; c += blockDim.x) {
        y[base + c] = row[c] * inv;
    }
}

extern "C" void softmax_cuda(const float *x, float *y, int rows, int cols) {
    if (cols > 1024) {
        fprintf(stderr, "[softmax_cuda] cols=%d too large for current teaching kernel (max 1024)\n", cols);
        exit(EXIT_FAILURE);
    }

    size_t bytes = (size_t)rows * (size_t)cols * sizeof(float);
    float *d_x = nullptr;
    float *d_y = nullptr;

    CUDA_CHECK(cudaMalloc(&d_x, bytes));
    CUDA_CHECK(cudaMalloc(&d_y, bytes));
    CUDA_CHECK(cudaMemcpy(d_x, x, bytes, cudaMemcpyHostToDevice));

    int threads = 256;
    size_t smem_bytes = (size_t)cols * sizeof(float) + (size_t)threads * sizeof(float);
    softmax_kernel<<<rows, threads, smem_bytes>>>(d_x, d_y, rows, cols);
    CUDA_CHECK_KERNEL();

    CUDA_CHECK(cudaMemcpy(y, d_y, bytes, cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaFree(d_x));
    CUDA_CHECK(cudaFree(d_y));
}
