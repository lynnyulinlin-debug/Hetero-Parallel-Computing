// gemm_serial.c —— GEMM 串行基线。
#include "gemm.h"
#include <stddef.h>

void gemm_serial(const float *a, const float *b, float *c, int n) {
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
