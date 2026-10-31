#ifndef HETEROPAR_GEMM_H
#define HETEROPAR_GEMM_H

#ifdef __cplusplus
extern "C" {
#endif

void gemm_serial(const float *a, const float *b, float *c, int n);
void gemm_openmp(const float *a, const float *b, float *c, int n);

#ifdef USE_CUDA
void gemm_cuda(const float *a, const float *b, float *c, int n);
#endif

#ifdef __cplusplus
}
#endif

#endif  // HETEROPAR_GEMM_H
