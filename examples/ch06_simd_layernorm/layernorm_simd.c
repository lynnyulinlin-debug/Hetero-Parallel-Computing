#include "layernorm.h"

#include <math.h>
#include <stddef.h>
#include <omp.h>

void layernorm_simd(void *arg) {
    const layernorm_ctx_t *ctx = (const layernorm_ctx_t *)arg;
    for (int r = 0; r < ctx->rows; ++r) {
        const float *in = ctx->input + (size_t)r * (size_t)ctx->cols;
        float *out = ctx->output + (size_t)r * (size_t)ctx->cols;

        double sum = 0.0;
        double sumsq = 0.0;
#pragma omp simd reduction(+:sum, sumsq)
        for (int c = 0; c < ctx->cols; ++c) {
            double x = in[c];
            sum += x;
            sumsq += x * x;
        }

        double mean = sum / (double)ctx->cols;
        double var = sumsq / (double)ctx->cols - mean * mean;
        float inv_std = 1.0f / sqrtf((float)var + ctx->eps);
#pragma omp simd
        for (int c = 0; c < ctx->cols; ++c) {
            out[c] = (in[c] - (float)mean) * inv_std;
        }
    }
}
