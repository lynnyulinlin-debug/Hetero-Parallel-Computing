#include "pipeline.h"

#include <math.h>
#include <stdlib.h>

void *pipeline_alloc_host(size_t bytes) {
    return malloc(bytes);
}

void pipeline_free_host(void *ptr) {
    free(ptr);
}

static inline float transform(float x, float scale, float bias) {
    return tanhf(scale * x + bias);
}

void pipeline_serial(void *arg) {
    const pipeline_ctx_t *ctx = (const pipeline_ctx_t *)arg;
    for (int i = 0; i < ctx->n; ++i) {
        ctx->output[i] = transform(ctx->input[i], ctx->scale, ctx->bias);
    }
}
