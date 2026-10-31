// main.c —— GEMM 对比驱动：串行 vs OpenMP vs CUDA。
// 流程：构造输入 → 串行算出黄金参考 → 各实现计时并校验结果 → 报告加速比。
#include <stdio.h>
#include <stdlib.h>

#include "gemm.h"
#include "../common/bench.h"
#include "../common/verify.h"

#ifdef USE_CUDA
#include <cuda_runtime.h>
#endif

typedef struct {
    const float *a;
    const float *b;
    float       *c;
    int          n;
    void (*fn)(const float *, const float *, float *, int);
} gemm_ctx_t;

static void run_impl(void *p) {
    gemm_ctx_t *ctx = (gemm_ctx_t *)p;
    ctx->fn(ctx->a, ctx->b, ctx->c, ctx->n);
}

static void fill_inputs(float *a, float *b, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            size_t idx = (size_t)i * (size_t)n + (size_t)j;
            a[idx] = 0.01f * (float)(((i * 3 + j * 5) % 11) - 5);
            b[idx] = 0.01f * (float)(((i * 7 + j * 2) % 13) - 6);
        }
    }
}

static void verify_matrix(const char *name, const float *got, const float *ref, int n) {
    if (verify_array_f32(got, ref, n * n, 1e-3f, 1e-3f) == 0) {
        printf("[verify] %-24s PASS\n", name);
        return;
    }
    printf("[verify] %-24s FAIL\n", name);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 512;
    if (n <= 0) {
        fprintf(stderr, "n must be positive\n");
        return EXIT_FAILURE;
    }
    printf("gemm: n=%d (A/B/C each %.1f MB)\n", n, (double)n * n * sizeof(float) / 1e6);

    size_t elems = (size_t)n * (size_t)n;
    float *a   = (float *)malloc(sizeof(float) * elems);
    float *b   = (float *)malloc(sizeof(float) * elems);
    float *c   = (float *)malloc(sizeof(float) * elems);
    float *ref = (float *)malloc(sizeof(float) * elems);
    if (!a || !b || !c || !ref) {
        fprintf(stderr, "allocation failed\n");
        free(a); free(b); free(c); free(ref);
        return EXIT_FAILURE;
    }

    fill_inputs(a, b, n);

    gemm_serial(a, b, ref, n);

    gemm_ctx_t cs = {a, b, c, n, gemm_serial};
    bench_result_t r_serial = bench_run("gemm_serial", 1, 5, run_impl, &cs);
    bench_report(&r_serial);
    verify_matrix("gemm_serial", c, ref, n);

    gemm_ctx_t co = {a, b, c, n, gemm_openmp};
    bench_result_t r_omp = bench_run("gemm_openmp", 1, 5, run_impl, &co);
    bench_report(&r_omp);
    verify_matrix("gemm_openmp", c, ref, n);
    bench_speedup(&r_serial, &r_omp);

#ifdef USE_CUDA
    int device_count = 0;
    cudaError_t dev_err = cudaGetDeviceCount(&device_count);
    if (dev_err != cudaSuccess || device_count <= 0) {
        printf("[info] CUDA executable but no available device; skipping CUDA GEMM test\n");
    } else {
        gemm_ctx_t cu = {a, b, c, n, gemm_cuda};
        bench_result_t r_cuda = bench_run("gemm_cuda", 1, 5, run_impl, &cu);
        bench_report(&r_cuda);
        verify_matrix("gemm_cuda", c, ref, n);
        bench_speedup(&r_serial, &r_cuda);
    }
#else
    printf("[info] CUDA 未编译（用 `make USE_CUDA=1` 启用）\n");
#endif

    free(a);
    free(b);
    free(c);
    free(ref);
    return 0;
}
