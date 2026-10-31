#ifndef HETEROPAR_TASK_POOL_H
#define HETEROPAR_TASK_POOL_H

typedef struct {
    int n;
    int chunk;
    int workers;
    int repeat;
    const float *input;
    float *output;
} task_pool_ctx_t;

void task_pool_serial(void *ctx);
void task_pool_threaded(void *ctx);

#endif  // HETEROPAR_TASK_POOL_H
