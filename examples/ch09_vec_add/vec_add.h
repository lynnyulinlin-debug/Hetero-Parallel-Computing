// vec_add.h —— 向量加法示例的公共声明：c = a + b
// 三种实现（serial / OpenMP / CUDA）共享同一签名，便于横向对比与结果校验。
#ifndef CH09_VEC_ADD_H
#define CH09_VEC_ADD_H

#ifdef __cplusplus
extern "C" {
#endif

// 串行实现（黄金参考）。
void vec_add_serial(const float *a, const float *b, float *c, int n);

// OpenMP 实现。
void vec_add_openmp(const float *a, const float *b, float *c, int n);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
// CUDA 实现（含 H2D/D2H 传输，单位墙钟可与 CPU 实现直接比较）。
// 仅在用 nvcc 编译时可见。
void vec_add_cuda(const float *a, const float *b, float *c, int n);
#endif

#endif  // CH09_VEC_ADD_H
