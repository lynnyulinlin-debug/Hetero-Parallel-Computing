#include "pipeline.h"
#include "../common/cuda_helpers.cuh"

#include <cmath>
#include <cstdio>
#include <cstdlib>

__global__ static void transform_kernel(const float *input, float *output,
                                        float scale, float bias, int n) {
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    if (idx < n) {
        output[idx] = tanhf(scale * input[idx] + bias);
    }
}

void *pipeline_alloc_host(size_t bytes) {
    void *ptr = NULL;
    CUDA_CHECK(cudaMallocHost(&ptr, bytes));
    return ptr;
}

void pipeline_free_host(void *ptr) {
    CUDA_CHECK(cudaFreeHost(ptr));
}

void pipeline_cuda(void *arg) {
    const pipeline_ctx_t *ctx = (const pipeline_ctx_t *)arg;
    int chunk = ctx->chunk > 0 ? ctx->chunk : 1;
    int streams = ctx->streams > 0 ? ctx->streams : 1;

    typedef struct {
        float *d_in;
        float *d_out;
        cudaStream_t stream;
        cudaEvent_t done;
    } Slot;

    Slot *slots = (Slot *)calloc((size_t)streams, sizeof(Slot));
    if (!slots) {
        fprintf(stderr, "[pipeline] out of memory\n");
        std::exit(EXIT_FAILURE);
    }

    for (int s = 0; s < streams; ++s) {
        CUDA_CHECK(cudaStreamCreate(&slots[s].stream));
        CUDA_CHECK(cudaEventCreateWithFlags(&slots[s].done, cudaEventDisableTiming));
        CUDA_CHECK(cudaMalloc(&slots[s].d_in, (size_t)chunk * sizeof(float)));
        CUDA_CHECK(cudaMalloc(&slots[s].d_out, (size_t)chunk * sizeof(float)));
    }

    int chunk_id = 0;
    for (int begin = 0; begin < ctx->n; begin += chunk, ++chunk_id) {
        int s = chunk_id % streams;
        if (chunk_id >= streams) {
            CUDA_CHECK(cudaEventSynchronize(slots[s].done));
        }
        int len = (begin + chunk <= ctx->n) ? chunk : (ctx->n - begin);
        CUDA_CHECK(cudaMemcpyAsync(slots[s].d_in, ctx->input + begin,
                                   (size_t)len * sizeof(float),
                                   cudaMemcpyHostToDevice, slots[s].stream));
        int block = 256;
        int grid = (len + block - 1) / block;
        transform_kernel<<<grid, block, 0, slots[s].stream>>>(
            slots[s].d_in, slots[s].d_out, ctx->scale, ctx->bias, len);
        CUDA_CHECK(cudaPeekAtLastError());
        CUDA_CHECK(cudaMemcpyAsync(ctx->output + begin, slots[s].d_out,
                                   (size_t)len * sizeof(float),
                                   cudaMemcpyDeviceToHost, slots[s].stream));
        CUDA_CHECK(cudaEventRecord(slots[s].done, slots[s].stream));
    }

    for (int s = 0; s < streams; ++s) {
        CUDA_CHECK(cudaEventSynchronize(slots[s].done));
        CUDA_CHECK(cudaFree(slots[s].d_in));
        CUDA_CHECK(cudaFree(slots[s].d_out));
        CUDA_CHECK(cudaEventDestroy(slots[s].done));
        CUDA_CHECK(cudaStreamDestroy(slots[s].stream));
    }

    free(slots);
}
