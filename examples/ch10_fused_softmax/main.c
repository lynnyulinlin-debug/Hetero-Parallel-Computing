// main.c —— Softmax 对比驱动：物化中间张量 vs 融合 CPU/OpenMP/CUDA。
#include <stdio.h>
#include <stdlib.h>

#include "softmax.h"
#include "../common/bench.h"
#include "../common/verify.h"

#ifdef USE_CUDA
#include <cuda_runtime.h>
#endif

typedef struct {
    const float *x;
    float       *y;
    int          rows;
    int          cols;
    void (*fn)(const float *, float *, int, int);
} softmax_ctx_t;

typedef struct {
    const float *x;
    float       *y;
    float       *intermediate;
    int          rows;
    int          cols;
} unfused_softmax_ctx_t;

static void run_impl(void *p) {
    softmax_ctx_t *ctx = (softmax_ctx_t *)p;
    ctx->fn(ctx->x, ctx->y, ctx->rows, ctx->cols);
}

static void run_unfused(void *p) {
    unfused_softmax_ctx_t *ctx = (unfused_softmax_ctx_t *)p;
    softmax_unfused(ctx->x, ctx->y, ctx->intermediate, ctx->rows, ctx->cols);
}

static void fill_inputs(float *x, int rows, int cols) {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            size_t idx = (size_t)r * (size_t)cols + (size_t)c;
            x[idx] = 0.01f * (float)(((r * 17 + c * 13) % 200) - 100);
        }
    }
}

static void verify_matrix(const char *name, const float *got, const float *ref, int rows, int cols) {
    if (verify_array_f32(got, ref, rows * cols, 1e-3f, 1e-4f) == 0) {
        printf("[verify] %-24s PASS\n", name);
        return;
    }
    printf("[verify] %-24s FAIL\n", name);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int rows = (argc > 1) ? atoi(argv[1]) : 2048;
    int cols = (argc > 2) ? atoi(argv[2]) : 1024;
    if (rows <= 0 || cols <= 0) {
        fprintf(stderr, "rows/cols must be positive\n");
        return EXIT_FAILURE;
    }
    printf("fused_softmax: rows=%d cols=%d (input %.1f MB)\n",
           rows, cols, (double)rows * cols * sizeof(float) / 1e6);

    size_t elems = (size_t)rows * (size_t)cols;
    float *x   = (float *)malloc(sizeof(float) * elems);
    float *y   = (float *)malloc(sizeof(float) * elems);
    float *ref = (float *)malloc(sizeof(float) * elems);
    float *intermediate = (float *)malloc(sizeof(float) * elems);
    if (!x || !y || !ref || !intermediate) {
        fprintf(stderr, "allocation failed\n");
        free(x); free(y); free(ref); free(intermediate);
        return EXIT_FAILURE;
    }

    fill_inputs(x, rows, cols);

    softmax_serial(x, ref, rows, cols);

    unfused_softmax_ctx_t cu = {x, y, intermediate, rows, cols};
    bench_result_t r_unfused = bench_run("softmax_unfused", 1, 5, run_unfused, &cu);
    bench_report(&r_unfused);
    verify_matrix("softmax_unfused", y, ref, rows, cols);

    softmax_ctx_t cs = {x, y, rows, cols, softmax_serial};
    bench_result_t r_serial = bench_run("softmax_serial", 1, 5, run_impl, &cs);
    bench_report(&r_serial);
    verify_matrix("softmax_serial", y, ref, rows, cols);
    bench_speedup(&r_unfused, &r_serial);

    softmax_ctx_t co = {x, y, rows, cols, softmax_openmp};
    bench_result_t r_omp = bench_run("softmax_openmp", 1, 5, run_impl, &co);
    bench_report(&r_omp);
    verify_matrix("softmax_openmp", y, ref, rows, cols);
    bench_speedup(&r_serial, &r_omp);

#ifdef USE_CUDA
    int device_count = 0;
    cudaError_t dev_err = cudaGetDeviceCount(&device_count);
    if (dev_err != cudaSuccess || device_count <= 0) {
        printf("[info] CUDA 可执行但当前环境没有可用设备，跳过 CUDA softmax 测试\n");
    } else {
        softmax_ctx_t cu = {x, y, rows, cols, softmax_cuda};
        bench_result_t r_cuda = bench_run("softmax_cuda", 1, 5, run_impl, &cu);
        bench_report(&r_cuda);
        verify_matrix("softmax_cuda", y, ref, rows, cols);
        bench_speedup(&r_serial, &r_cuda);
    }
#else
    printf("[info] CUDA 未编译（用 `make USE_CUDA=1` 启用）\n");
#endif

    free(x);
    free(y);
    free(ref);
    free(intermediate);
    return 0;
}
