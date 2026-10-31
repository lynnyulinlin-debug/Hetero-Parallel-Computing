#ifndef HETEROPAR_SOFTMAX_H
#define HETEROPAR_SOFTMAX_H

#ifdef __cplusplus
extern "C" {
#endif

void softmax_serial(const float *x, float *y, int rows, int cols);
void softmax_unfused(const float *x, float *y, float *intermediate, int rows, int cols);
void softmax_openmp(const float *x, float *y, int rows, int cols);

#ifdef USE_CUDA
void softmax_cuda(const float *x, float *y, int rows, int cols);
#endif

#ifdef __cplusplus
}
#endif

#endif  // HETEROPAR_SOFTMAX_H
