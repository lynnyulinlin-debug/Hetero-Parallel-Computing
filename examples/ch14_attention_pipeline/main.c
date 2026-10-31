// main.c —— Attention 对比驱动：串行 vs OpenMP vs CUDA。
#include <stdio.h>
#include <stdlib.h>

#include "attention.h"
#include "../common/bench.h"
#include "../common/verify.h"

#ifdef USE_CUDA
#include <cuda_runtime.h>
#endif

typedef struct {
    const float *q;
    const float *k;
    const float *v;
    float       *o;
    int          rows;
    int          dim;
    void (*fn)(const float *, const float *, const float *, float *, int, int);
} attention_ctx_t;

static void run_impl(void *p) {
    attention_ctx_t *ctx = (attention_ctx_t *)p;
    ctx->fn(ctx->q, ctx->k, ctx->v, ctx->o, ctx->rows, ctx->dim);
}

static void fill_inputs(float *q, float *k, float *v, int rows, int dim) {
    for (int i = 0; i < rows; ++i) {
        for (int d = 0; d < dim; ++d) {
            size_t idx = (size_t)i * (size_t)dim + (size_t)d;
            q[idx] = 0.01f * (float)(((i * 3 + d * 5) % 23) - 11);
            k[idx] = 0.01f * (float)(((i * 7 + d * 2) % 19) - 9);
            v[idx] = 0.01f * (float)(((i * 11 + d * 13) % 29) - 14);
        }
    }
}

static void verify_matrix(const char *name, const float *got, const float *ref, int rows, int dim) {
    if (verify_array_f32(got, ref, rows * dim, 1e-3f, 1e-3f) == 0) {
        printf("[verify] %-24s PASS\n", name);
        return;
    }
    printf("[verify] %-24s FAIL\n", name);
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    int rows = (argc > 1) ? atoi(argv[1]) : 512;
    int dim  = (argc > 2) ? atoi(argv[2]) : 64;
    if (rows <= 0 || dim <= 0) {
        fprintf(stderr, "rows/dim must be positive\n");
        return EXIT_FAILURE;
    }
    printf("attention_pipeline: rows=%d dim=%d (Q/K/V each %.1f MB)\n",
           rows, dim, (double)rows * dim * sizeof(float) / 1e6);

    size_t elems = (size_t)rows * (size_t)dim;
    float *q   = (float *)malloc(sizeof(float) * elems);
    float *k   = (float *)malloc(sizeof(float) * elems);
    float *v   = (float *)malloc(sizeof(float) * elems);
    float *o   = (float *)malloc(sizeof(float) * elems);
    float *ref = (float *)malloc(sizeof(float) * elems);
    if (!q || !k || !v || !o || !ref) {
        fprintf(stderr, "allocation failed\n");
        free(q); free(k); free(v); free(o); free(ref);
        return EXIT_FAILURE;
    }

    fill_inputs(q, k, v, rows, dim);

    attention_serial(q, k, v, ref, rows, dim);

    attention_ctx_t cs = {q, k, v, o, rows, dim, attention_serial};
    bench_result_t r_serial = bench_run("attention_serial", 1, 3, run_impl, &cs);
    bench_report(&r_serial);
    verify_matrix("attention_serial", o, ref, rows, dim);

    attention_ctx_t co = {q, k, v, o, rows, dim, attention_openmp};
    bench_result_t r_omp = bench_run("attention_openmp", 1, 3, run_impl, &co);
    bench_report(&r_omp);
    verify_matrix("attention_openmp", o, ref, rows, dim);
    bench_speedup(&r_serial, &r_omp);

#ifdef USE_CUDA
    int device_count = 0;
    cudaError_t dev_err = cudaGetDeviceCount(&device_count);
    if (dev_err != cudaSuccess || device_count <= 0) {
        printf("[info] CUDA 可执行但当前环境没有可用设备，跳过 CUDA attention 测试\n");
    } else {
        attention_ctx_t cu = {q, k, v, o, rows, dim, attention_cuda};
        bench_result_t r_cuda = bench_run("attention_cuda", 1, 3, run_impl, &cu);
        bench_report(&r_cuda);
        verify_matrix("attention_cuda", o, ref, rows, dim);
        bench_speedup(&r_serial, &r_cuda);
    }
#else
    printf("[info] CUDA 未编译（用 `make USE_CUDA=1` 启用）\n");
#endif

    free(q);
    free(k);
    free(v);
    free(o);
    free(ref);
    return 0;
}
