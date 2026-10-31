// attention_cuda.cu —— FlashAttention 风格教学版。
// 一个 block 处理一个 query row：先做 causal scores，再做稳定 softmax，最后汇聚 V。
#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include "attention.h"
#include "../common/cuda_helpers.cuh"

__global__ void attention_kernel(const float *q, const float *k, const float *v,
                                 float *o, int rows, int dim) {
    extern __shared__ float smem[];
    float *scores = smem;
    float *scratch = scores + rows;

    int i = blockIdx.x;
    if (i >= rows) return;

    int tid = threadIdx.x;
    float inv_sqrt_dim = 1.0f / sqrtf((float)dim);

    const float *qi = q + (size_t)i * (size_t)dim;

    for (int j = tid; j <= i; j += blockDim.x) {
        const float *kj = k + (size_t)j * (size_t)dim;
        float dot = 0.0f;
        for (int d = 0; d < dim; ++d) {
            dot += qi[d] * kj[d];
        }
        scores[j] = dot * inv_sqrt_dim;
    }
    __syncthreads();

    float local_max = -FLT_MAX;
    for (int j = tid; j <= i; j += blockDim.x) {
        local_max = fmaxf(local_max, scores[j]);
    }
    scratch[tid] = local_max;
    __syncthreads();
    for (int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) scratch[tid] = fmaxf(scratch[tid], scratch[tid + s]);
        __syncthreads();
    }
    float maxv = scratch[0];

    float local_sum = 0.0f;
    for (int j = tid; j <= i; j += blockDim.x) {
        float e = expf(scores[j] - maxv);
        scores[j] = e;
        local_sum += e;
    }
    scratch[tid] = local_sum;
    __syncthreads();
    for (int s = blockDim.x / 2; s > 0; s >>= 1) {
        if (tid < s) scratch[tid] += scratch[tid + s];
        __syncthreads();
    }
    float inv_sum = 1.0f / scratch[0];

    for (int j = tid; j <= i; j += blockDim.x) {
        scores[j] *= inv_sum;
    }
    __syncthreads();

    float *oi = o + (size_t)i * (size_t)dim;
    for (int d = tid; d < dim; d += blockDim.x) {
        float acc = 0.0f;
        for (int j = 0; j <= i; ++j) {
            const float *vj = v + (size_t)j * (size_t)dim;
            acc += scores[j] * vj[d];
        }
        oi[d] = acc;
    }
}

extern "C" void attention_cuda(const float *q, const float *k, const float *v,
                               float *o, int rows, int dim) {
    if (rows > 4096) {
        fprintf(stderr, "[attention_cuda] rows=%d too large for current teaching kernel (max 4096)\n", rows);
        exit(EXIT_FAILURE);
    }

    size_t bytes = (size_t)rows * (size_t)dim * sizeof(float);
    float *d_q = nullptr;
    float *d_k = nullptr;
    float *d_v = nullptr;
    float *d_o = nullptr;

    CUDA_CHECK(cudaMalloc(&d_q, bytes));
    CUDA_CHECK(cudaMalloc(&d_k, bytes));
    CUDA_CHECK(cudaMalloc(&d_v, bytes));
    CUDA_CHECK(cudaMalloc(&d_o, bytes));
    CUDA_CHECK(cudaMemcpy(d_q, q, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_k, k, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_v, v, bytes, cudaMemcpyHostToDevice));

    int threads = 256;
    size_t smem_bytes = (size_t)rows * sizeof(float) + (size_t)threads * sizeof(float);
    attention_kernel<<<rows, threads, smem_bytes>>>(d_q, d_k, d_v, d_o, rows, dim);
    CUDA_CHECK_KERNEL();

    CUDA_CHECK(cudaMemcpy(o, d_o, bytes, cudaMemcpyDeviceToHost));
    CUDA_CHECK(cudaFree(d_q));
    CUDA_CHECK(cudaFree(d_k));
    CUDA_CHECK(cudaFree(d_v));
    CUDA_CHECK(cudaFree(d_o));
}
