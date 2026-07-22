// reduce.h —— 归约示例公共声明：sum = reduce(a)
// 四种实现共享同一签名，便于横向对比与结果校验。
#ifndef CH09_REDUCE_H
#define CH09_REDUCE_H

#ifdef __cplusplus
extern "C" {
#endif

void reduce_serial(const float *a, float *sum, int n);
void reduce_openmp(const float *a, float *sum, int n);

#ifdef __cplusplus
}
#endif

#ifdef USE_CUDA
#ifdef __cplusplus
extern "C" {
#endif
void reduce_cuda_naive(const float *a, float *sum, int n);
void reduce_cuda_shared(const float *a, float *sum, int n);
#ifdef __cplusplus
}
#endif
#endif

#endif  // CH09_REDUCE_H
