#ifndef HETEROPAR_BLUR_H
#define HETEROPAR_BLUR_H

typedef struct {
    int width;
    int height;
    const float *input;
    float *output;
} blur_ctx_t;

void blur_serial(void *ctx);
void blur_openmp(void *ctx);

#endif  // HETEROPAR_BLUR_H
