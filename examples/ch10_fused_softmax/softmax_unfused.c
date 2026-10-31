// CPU baseline that materializes exp(x - max) before normalization.
#include "softmax.h"

#include <math.h>
#include <stddef.h>

void softmax_unfused(const float *x, float *y, float *intermediate, int rows, int cols) {
    for (int r = 0; r < rows; ++r) {
        const float *in = x + (size_t)r * (size_t)cols;
        float *tmp = intermediate + (size_t)r * (size_t)cols;
        float *out = y + (size_t)r * (size_t)cols;

        float maxv = in[0];
        for (int c = 1; c < cols; ++c) {
            if (in[c] > maxv) maxv = in[c];
        }

        for (int c = 0; c < cols; ++c) {
            tmp[c] = expf(in[c] - maxv);
        }

        float sum = 0.0f;
        for (int c = 0; c < cols; ++c) {
            sum += tmp[c];
        }

        float inv = 1.0f / sum;
        for (int c = 0; c < cols; ++c) {
            out[c] = tmp[c] * inv;
        }
    }
}
