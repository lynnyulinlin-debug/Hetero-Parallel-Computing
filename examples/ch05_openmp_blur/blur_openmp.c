#include "blur.h"

#include <omp.h>

static int clampi(int v, int lo, int hi) {
    return v < lo ? lo : (v > hi ? hi : v);
}

static float blur_pixel(const float *input, int width, int height, int x, int y) {
    float sum = 0.0f;
    for (int dy = -1; dy <= 1; ++dy) {
        int yy = clampi(y + dy, 0, height - 1);
        for (int dx = -1; dx <= 1; ++dx) {
            int xx = clampi(x + dx, 0, width - 1);
            sum += input[yy * width + xx];
        }
    }
    return sum / 9.0f;
}

void blur_openmp(void *arg) {
    const blur_ctx_t *ctx = (const blur_ctx_t *)arg;

#pragma omp parallel for schedule(static)
    for (int y = 0; y < ctx->height; ++y) {
        for (int x = 0; x < ctx->width; ++x) {
            ctx->output[y * ctx->width + x] =
                blur_pixel(ctx->input, ctx->width, ctx->height, x, y);
        }
    }
}
