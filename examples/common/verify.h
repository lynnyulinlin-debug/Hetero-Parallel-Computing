// verify.h —— 跨示例统一的结果校验工具（仅头文件）
//
// 设计动机：异构示例的核心风险是"算得快但算错了"。每个示例都用
// 串行实现作为黄金参考（golden reference），并行/GPU 结果必须与之逐元素
// 比对。浮点运算受累加顺序、FMA、低精度影响，因此用相对+绝对容差，
// 而非精确相等。

#ifndef HETEROPAR_VERIFY_H
#define HETEROPAR_VERIFY_H

#include <math.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

// 单元素近似相等：|a-b| <= atol + rtol*|b|
static inline int verify_close(double a, double b, double rtol, double atol) {
    return fabs(a - b) <= atol + rtol * fabs(b);
}

// 比对两个 float 数组。返回 0 表示通过，非 0 表示首个不匹配的下标+1。
// 打印最大绝对/相对误差，便于判断是"真错"还是"正常浮点偏差"。
static inline int verify_array_f32(const float *got, const float *ref, int n,
                                   float rtol, float atol) {
    int    first_bad = -1;
    double max_abs = 0.0, max_rel = 0.0;
    for (int i = 0; i < n; ++i) {
        double abs_err = fabs((double)got[i] - (double)ref[i]);
        double rel_err = abs_err / (fabs((double)ref[i]) + 1e-30);
        if (abs_err > max_abs) max_abs = abs_err;
        if (rel_err > max_rel) max_rel = rel_err;
        if (first_bad < 0 && !verify_close(got[i], ref[i], rtol, atol))
            first_bad = i;
    }
    if (first_bad >= 0) {
        printf("[verify] FAIL  first mismatch @%d: got=%g ref=%g  "
               "(max_abs=%g, max_rel=%g)\n",
               first_bad, got[first_bad], ref[first_bad], max_abs, max_rel);
        return first_bad + 1;
    }
    printf("[verify] PASS  (n=%d, max_abs=%g, max_rel=%g)\n", n, max_abs, max_rel);
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif  // HETEROPAR_VERIFY_H
