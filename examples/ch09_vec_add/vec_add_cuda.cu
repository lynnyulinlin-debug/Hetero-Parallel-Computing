// vec_add_cuda.cu —— CUDA 版向量加法。
// 演示完整的 Host↔Device 协同流程：分配设备内存 → H2D 拷贝 →
// 启动核函数 → D2H 拷贝 → 释放。这是第9章主-从内存模型的最小范例。
#include "vec_add.h"
#include "../common/cuda_helpers.cuh"

// 核函数：每个线程负责一个元素。全局线程索引 = blockIdx*blockDim + threadIdx。
__global__ void vec_add_kernel(const float *a, const float *b, float *c, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {                 // 边界检查：线程数通常向上取整，会有越界线程
        c[i] = a[i] + b[i];
    }
}

void vec_add_cuda(const float *a, const float *b, float *c, int n) {
    float *d_a = nullptr, *d_b = nullptr, *d_c = nullptr;
    size_t bytes = (size_t)n * sizeof(float);

    CUDA_CHECK(cudaMalloc(&d_a, bytes));
    CUDA_CHECK(cudaMalloc(&d_b, bytes));
    CUDA_CHECK(cudaMalloc(&d_c, bytes));

    CUDA_CHECK(cudaMemcpy(d_a, a, bytes, cudaMemcpyHostToDevice));
    CUDA_CHECK(cudaMemcpy(d_b, b, bytes, cudaMemcpyHostToDevice));

    int threads = 256;                          // 每 block 线程数（warp 的整数倍）
    int blocks  = (n + threads - 1) / threads;  // 向上取整覆盖所有元素
    vec_add_kernel<<<blocks, threads>>>(d_a, d_b, d_c, n);
    CUDA_CHECK_KERNEL();

    CUDA_CHECK(cudaMemcpy(c, d_c, bytes, cudaMemcpyDeviceToHost));

    CUDA_CHECK(cudaFree(d_a));
    CUDA_CHECK(cudaFree(d_b));
    CUDA_CHECK(cudaFree(d_c));
}
