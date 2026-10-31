#ifndef HETEROPAR_PIPELINE_H
#define HETEROPAR_PIPELINE_H

#include <stddef.h>

typedef struct {
    int n;
    int chunk;
    int streams;
    float scale;
    float bias;
    const float *input;
    float *output;
} pipeline_ctx_t;

void *pipeline_alloc_host(size_t bytes);
void pipeline_free_host(void *ptr);

void pipeline_serial(void *ctx);

#ifdef USE_CUDA
void pipeline_cuda(void *ctx);
#endif

#endif  // HETEROPAR_PIPELINE_H
