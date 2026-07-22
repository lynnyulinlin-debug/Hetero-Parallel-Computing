// cuda_helpers.cuh —— CUDA 示例通用辅助（错误检查 + 设备侧计时）
//
// 设计动机：
//   - CUDA API 静默失败是初学者最大的坑。CUDA_CHECK 宏强制检查每个返回值。
//   - 设备侧真实耗时必须用 cudaEvent 测量（核函数异步启动，主机墙钟会测偏）。
//     这与 bench.h 的主机侧计时互补，对应第10章工具链与第13章方法论。

#ifndef HETEROPAR_CUDA_HELPERS_CUH
#define HETEROPAR_CUDA_HELPERS_CUH

#include <cuda_runtime.h>
#include <cstdio>
#include <cstdlib>

// 检查任意 CUDA runtime 调用的返回码，出错即打印位置并退出。
#define CUDA_CHECK(call)                                                       \
    do {                                                                       \
        cudaError_t err__ = (call);                                            \
        if (err__ != cudaSuccess) {                                            \
            fprintf(stderr, "[CUDA] %s:%d  %s -> %s\n", __FILE__, __LINE__,    \
                    #call, cudaGetErrorString(err__));                         \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)

// 核函数启动后调用，捕获异步启动错误（启动配置非法等）。
#define CUDA_CHECK_KERNEL()                                                    \
    do {                                                                       \
        CUDA_CHECK(cudaGetLastError());                                        \
        CUDA_CHECK(cudaDeviceSynchronize());                                   \
    } while (0)

// 基于 cudaEvent 的设备侧计时器，测量 GPU 上的真实执行时间（毫秒）。
struct CudaTimer {
    cudaEvent_t start_, stop_;
    CudaTimer()  { CUDA_CHECK(cudaEventCreate(&start_)); CUDA_CHECK(cudaEventCreate(&stop_)); }
    ~CudaTimer() { cudaEventDestroy(start_); cudaEventDestroy(stop_); }
    void start() { CUDA_CHECK(cudaEventRecord(start_)); }
    float stop_ms() {
        CUDA_CHECK(cudaEventRecord(stop_));
        CUDA_CHECK(cudaEventSynchronize(stop_));
        float ms = 0.f;
        CUDA_CHECK(cudaEventElapsedTime(&ms, start_, stop_));
        return ms;
    }
};

#endif  // HETEROPAR_CUDA_HELPERS_CUH
