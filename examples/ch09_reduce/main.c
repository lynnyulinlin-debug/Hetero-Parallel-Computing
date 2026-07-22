// main.c —— 归约对比驱动：串行 vs OpenMP vs CUDA。
// 流程：构造输入 → 串行算出黄金参考 → 各实现计时并校验结果 → 报告加速比。
#include <stdio.h>
#include <stdlib.h>

#include "reduce.h"
#include "../common/bench.h"
#include "../common/verify.h"

#ifdef USE_CUDA
#include <cuda_runtime.h>
#endif

typedef struct {
    const float *a;
    float       *sum;
    int          n;
    void (*fn)(const float *, float *, int);
} reduce_ctx_t;

static void run_impl(void *p) {
    reduce_ctx_t *ctx = (reduce_ctx_t *)p;
    ctx->fn(ctx->a, ctx->sum, ctx->n);
}

static void verify_sum(const char *name, float got, float ref) {
    if (verify_close((double)got, (double)ref, 1e-3, 1e-3)) {
        printf("[verify] %-24s PASS  got=%g ref=%g\n", name, got, ref);
        return;
    }
    printf("[verify] %-24s FAIL  got=%g ref=%g\n", name, got, ref);
    exit(EXIT_FAILURE);
}

static void fill_input(float *a, int n) {
    for (int i = 0; i < n; ++i) {
        a[i] = 0.001f * (float)((i * 17) % 1000);
    }
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : (1 << 24);
    printf("reduce: n=%d (%.1f MB/input)\n", n, n * sizeof(float) / 1e6);

    float *a = (float *)malloc(sizeof(float) * n);
    if (!a) {
        fprintf(stderr, "alloc input failed\n");
        return EXIT_FAILURE;
    }
    fill_input(a, n);

    float ref = 0.0f;
    reduce_serial(a, &ref, n);

    float out = 0.0f;

    reduce_ctx_t cs = {a, &out, n, reduce_serial};
    bench_result_t r_serial = bench_run("reduce_serial", 2, 11, run_impl, &cs);
    bench_report(&r_serial);
    verify_sum("reduce_serial", out, ref);

    reduce_ctx_t co = {a, &out, n, reduce_openmp};
    bench_result_t r_omp = bench_run("reduce_openmp", 2, 11, run_impl, &co);
    bench_report(&r_omp);
    verify_sum("reduce_openmp", out, ref);
    bench_speedup(&r_serial, &r_omp);

#ifdef USE_CUDA
    int device_count = 0;
    cudaError_t dev_err = cudaGetDeviceCount(&device_count);
    if (dev_err != cudaSuccess || device_count <= 0) {
        printf("[info] CUDA 可执行但当前环境没有可用设备，跳过 CUDA 归约测试\n");
    } else {
        reduce_ctx_t cn = {a, &out, n, reduce_cuda_naive};
        bench_result_t r_cuda_naive = bench_run("reduce_cuda_naive", 2, 11, run_impl, &cn);
        bench_report(&r_cuda_naive);
        verify_sum("reduce_cuda_naive", out, ref);
        bench_speedup(&r_serial, &r_cuda_naive);

        reduce_ctx_t csd = {a, &out, n, reduce_cuda_shared};
        bench_result_t r_cuda_shared = bench_run("reduce_cuda_shared", 2, 11, run_impl, &csd);
        bench_report(&r_cuda_shared);
        verify_sum("reduce_cuda_shared", out, ref);
        bench_speedup(&r_serial, &r_cuda_shared);
    }
#else
    printf("[info] CUDA 未编译（用 `make USE_CUDA=1` 启用）\n");
#endif

    free(a);
    return 0;
}
