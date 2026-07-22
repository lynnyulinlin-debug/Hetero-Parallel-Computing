// reduce_openmp.c —— OpenMP 归约。
#include "reduce.h"

void reduce_openmp(const float *a, float *sum, int n) {
    float acc = 0.0f;
#pragma omp parallel for reduction(+:acc) schedule(static)
    for (int i = 0; i < n; ++i) {
        acc += a[i];
    }
    *sum = acc;
}
