// softmax_serial.c —— row-wise softmax 串行基线。
#include "softmax.h"

#include <math.h>
#include <stddef.h>

void softmax_serial(const float *x, float *y, int rows, int cols) {
    for (int r = 0; r < rows; ++r) {
        const float *in = x + (size_t)r * (size_t)cols;
        float *out = y + (size_t)r * (size_t)cols;

        float maxv = in[0];
        for (int c = 1; c < cols; ++c) {
            if (in[c] > maxv) maxv = in[c];
        }

        float sum = 0.0f;
        for (int c = 0; c < cols; ++c) {
            float e = expf(in[c] - maxv);
            out[c] = e;
            sum += e;
        }

        float inv = 1.0f / sum;
        for (int c = 0; c < cols; ++c) {
            out[c] *= inv;
        }
    }
}
