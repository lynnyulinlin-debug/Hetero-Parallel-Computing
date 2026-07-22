// reduce_serial.c —— 串行归约，作为黄金参考实现。
#include "reduce.h"

void reduce_serial(const float *a, float *sum, int n) {
    float acc = 0.0f;
    for (int i = 0; i < n; ++i) {
        acc += a[i];
    }
    *sum = acc;
}
