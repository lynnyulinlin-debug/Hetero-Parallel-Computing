// vec_add_openmp.c —— OpenMP 数据并行版向量加法。
// 关键：#pragma omp parallel for 将循环迭代静态切分给多个线程。
// 元素间无数据依赖，是最干净的数据并行（embarrassingly parallel）范例。
#include "vec_add.h"

void vec_add_openmp(const float *a, const float *b, float *c, int n) {
#pragma omp parallel for schedule(static)
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}
