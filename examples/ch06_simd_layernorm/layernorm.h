#ifndef HETEROPAR_LAYERNORM_H
#define HETEROPAR_LAYERNORM_H

typedef struct {
    int rows;
    int cols;
    float eps;
    const float *input;
    float *output;
} layernorm_ctx_t;

void layernorm_serial(void *ctx);
void layernorm_simd(void *ctx);

#endif  // HETEROPAR_LAYERNORM_H
