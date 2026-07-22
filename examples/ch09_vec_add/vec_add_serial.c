// vec_add_serial.c —— 串行向量加法，作为黄金参考实现。
#include "vec_add.h"

void vec_add_serial(const float *a, const float *b, float *c, int n) {
    for (int i = 0; i < n; ++i) {
        c[i] = a[i] + b[i];
    }
}
