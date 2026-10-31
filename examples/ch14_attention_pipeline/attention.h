#ifndef HETEROPAR_ATTENTION_H
#define HETEROPAR_ATTENTION_H

#ifdef __cplusplus
extern "C" {
#endif

void attention_serial(const float *q, const float *k, const float *v, float *o,
                      int rows, int dim);
void attention_openmp(const float *q, const float *k, const float *v, float *o,
                      int rows, int dim);

#ifdef USE_CUDA
void attention_cuda(const float *q, const float *k, const float *v, float *o,
                    int rows, int dim);
#endif

#ifdef __cplusplus
}
#endif

#endif  // HETEROPAR_ATTENTION_H
