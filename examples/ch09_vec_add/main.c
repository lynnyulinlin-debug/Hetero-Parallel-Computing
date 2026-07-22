// main.c —— 向量加法对比驱动：串行 vs OpenMP vs CUDA。
// 流程：构造输入 → 串行算出黄金参考 → 各实现计时并校验结果 → 报告加速比。
// 体现教程主线：同一问题、多种实现、统一计时口径、强制结果校验。
#include <stdio.h>
#include <stdlib.h>

#include "vec_add.h"
#include "../common/bench.h"
#include "../common/verify.h"

// CUDA 实现用 C++ 链接，这里手动声明以便在 C 驱动里调用（若编译了 CUDA）。
#ifdef USE_CUDA
extern void vec_add_cuda(const float *a, const float *b, float *c, int n);
#endif

typedef struct {
    const float *a, *b;
    float       *c;
    int          n;
    void (*fn)(const float *, const float *, float *, int);
} vec_ctx_t;

static void run_impl(void *p) {
    vec_ctx_t *ctx = (vec_ctx_t *)p;
    ctx->fn(ctx->a, ctx->b, ctx->c, ctx->n);
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : (1 << 24);  // 默认 ~1600 万元素
    printf("vec_add: n=%d (%.1f MB/array)\n", n, n * sizeof(float) / 1e6);

    float *a   = (float *)malloc(sizeof(float) * n);
    float *b   = (float *)malloc(sizeof(float) * n);
    float *c   = (float *)malloc(sizeof(float) * n);
    float *ref = (float *)malloc(sizeof(float) * n);
    for (int i = 0; i < n; ++i) { a[i] = (float)(i % 100); b[i] = (float)(i % 7); }

    // 1) 黄金参考
    vec_add_serial(a, b, ref, n);

    // 2) 串行计时（baseline）
    vec_ctx_t cs = {a, b, c, n, vec_add_serial};
    bench_result_t r_serial = bench_run("vec_add_serial", 2, 11, run_impl, &cs);
    bench_report(&r_serial);

    // 3) OpenMP
    vec_ctx_t co = {a, b, c, n, vec_add_openmp};
    bench_result_t r_omp = bench_run("vec_add_openmp", 2, 11, run_impl, &co);
    bench_report(&r_omp);
    verify_array_f32(c, ref, n, 0.f, 0.f);  // 整数和，应精确相等
    bench_speedup(&r_serial, &r_omp);

#ifdef USE_CUDA
    // 4) CUDA（含传输开销，反映真实端到端代价）
    vec_ctx_t cu = {a, b, c, n, vec_add_cuda};
    bench_result_t r_cuda = bench_run("vec_add_cuda", 2, 11, run_impl, &cu);
    bench_report(&r_cuda);
    verify_array_f32(c, ref, n, 0.f, 0.f);
    bench_speedup(&r_serial, &r_cuda);
#else
    printf("[info] CUDA 未编译（用 `make USE_CUDA=1` 启用）\n");
#endif

    free(a); free(b); free(c); free(ref);
    return 0;
}
