// gemm_openmp.c —— GEMM 的 OpenMP 版本。
#include <omp.h>
#include <stddef.h>

#include "gemm.h"

void gemm_openmp(const float *a, const float *b, float *c, int n) {
    #pragma omp parallel for collapse(2) schedule(static)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            float sum = 0.0f;
            const float *row_a = a + (size_t)i * (size_t)n;
            for (int k = 0; k < n; ++k) {
                sum += row_a[k] * b[(size_t)k * (size_t)n + (size_t)j];
            }
            c[(size_t)i * (size_t)n + (size_t)j] = sum;
        }
    }
}
