// bench.h —— 跨示例统一的基准测试工具（仅头文件，C/C++ 通用）
//
// 设计目标（呼应第13章性能分析方法论）：
//   1. 默认做 warmup，排除首次运行的 JIT / 上下文创建 / 缓存冷启动开销
//   2. 多次运行取中位数，而非单次或均值，抵抗偶发抖动
//   3. 计时口径统一，让 serial / OpenMP / CUDA 等不同实现可横向对比
//
// 用法：
//   bench_result_t r = bench_run("vec_add_serial", 5, 20, my_fn, ctx);
//   bench_report(&r);
//
// 注意：本头文件只负责"主机侧墙钟时间"。GPU 设备侧时间请用 CUDA event
//       （见 examples/common/cuda_helpers.cuh 中的 cuda_timer）。

#ifndef HETEROPAR_BENCH_H
#define HETEROPAR_BENCH_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    const char *name;
    double median_ms;   // 中位数（主报告指标）
    double min_ms;
    double max_ms;
    int    iters;
} bench_result_t;

// 待测函数签名：接收一个不透明上下文指针，执行一次完整计算。
typedef void (*bench_fn_t)(void *ctx);

// 返回当前时刻的单调墙钟（毫秒）。
static inline double bench_now_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1e3 + ts.tv_nsec * 1e-6;
}

static int bench__cmp_double(const void *a, const void *b) {
    double da = *(const double *)a, db = *(const double *)b;
    return (da > db) - (da < db);
}

// 运行 warmup 次预热（不计时），再运行 iters 次正式计时，返回统计量。
static inline bench_result_t bench_run(const char *name, int warmup, int iters,
                                       bench_fn_t fn, void *ctx) {
    for (int i = 0; i < warmup; ++i) fn(ctx);

    double *samples = (double *)malloc(sizeof(double) * iters);
    for (int i = 0; i < iters; ++i) {
        double t0 = bench_now_ms();
        fn(ctx);
        samples[i] = bench_now_ms() - t0;
    }
    qsort(samples, iters, sizeof(double), bench__cmp_double);

    bench_result_t r;
    r.name      = name;
    r.iters     = iters;
    r.min_ms    = samples[0];
    r.max_ms    = samples[iters - 1];
    r.median_ms = (iters & 1) ? samples[iters / 2]
                              : 0.5 * (samples[iters / 2 - 1] + samples[iters / 2]);
    free(samples);
    return r;
}

static inline void bench_report(const bench_result_t *r) {
    printf("[bench] %-24s median=%8.3f ms  (min=%8.3f, max=%8.3f, n=%d)\n",
           r->name, r->median_ms, r->min_ms, r->max_ms, r->iters);
}

// 相对加速比报告：以 baseline 为 1.0x。
static inline void bench_speedup(const bench_result_t *baseline,
                                 const bench_result_t *candidate) {
    printf("[speedup] %-24s %6.2fx  vs  %s\n",
           candidate->name, baseline->median_ms / candidate->median_ms,
           baseline->name);
}

#ifdef __cplusplus
}
#endif

#endif  // HETEROPAR_BENCH_H
