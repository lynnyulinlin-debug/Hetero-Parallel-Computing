#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/bench.h"
#include "../common/verify.h"
#include "pipeline.h"

static void fill_input(float *x, int n) {
    for (int i = 0; i < n; ++i) {
        x[i] = 0.001f * (float)i + 0.25f * sinf(0.002f * (float)i);
    }
}

static void usage(const char *argv0) {
    fprintf(stderr,
            "Usage: %s [n] [chunk] [streams]\n"
            "  n       : number of elements (default 16777216)\n"
            "  chunk   : chunk size (default 1048576)\n"
            "  streams : CUDA streams (default 4)\n",
            argv0);
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : (1 << 24);
    int chunk = (argc > 2) ? atoi(argv[2]) : (1 << 20);
    int streams = (argc > 3) ? atoi(argv[3]) : 4;
    float scale = 0.75f;
    float bias = 0.10f;

    if (n <= 0 || chunk <= 0 || streams <= 0) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    printf("=== Chapter 7: Host Overlap ===\n");
    printf("n=%d chunk=%d streams=%d scale=%.2f bias=%.2f\n",
           n, chunk, streams, scale, bias);

    float *input = (float *)pipeline_alloc_host((size_t)n * sizeof(float));
    float *serial_out = (float *)pipeline_alloc_host((size_t)n * sizeof(float));
    float *async_out = (float *)pipeline_alloc_host((size_t)n * sizeof(float));
    if (!input || !serial_out || !async_out) {
        fprintf(stderr, "out of memory\n");
        return EXIT_FAILURE;
    }

    fill_input(input, n);

    pipeline_ctx_t serial_ctx = {
        .n = n,
        .chunk = chunk,
        .streams = 1,
        .scale = scale,
        .bias = bias,
        .input = input,
        .output = serial_out,
    };

    bench_result_t r_serial = bench_run("pipeline_serial", 2, 5, pipeline_serial, &serial_ctx);
    bench_report(&r_serial);

    // CPU-only 构建也验证 Host 侧参考路径，避免该示例只剩一个性能 smoke test。
    for (int i = 0; i < n; ++i) {
        async_out[i] = tanhf(input[i] * scale + bias);
    }
    if (verify_array_f32(serial_out, async_out, n, 1e-5f, 1e-5f) != 0) {
        pipeline_free_host(input);
        pipeline_free_host(serial_out);
        pipeline_free_host(async_out);
        return EXIT_FAILURE;
    }

#ifdef USE_CUDA
    pipeline_ctx_t async_ctx = serial_ctx;
    async_ctx.streams = streams;
    async_ctx.output = async_out;
    bench_result_t r_async = bench_run("pipeline_async", 2, 5, pipeline_cuda, &async_ctx);
    bench_report(&r_async);
    bench_speedup(&r_serial, &r_async);
    if (verify_array_f32(async_out, serial_out, n, 1e-5f, 1e-5f) != 0) {
        pipeline_free_host(input);
        pipeline_free_host(serial_out);
        pipeline_free_host(async_out);
        return EXIT_FAILURE;
    }
#else
    printf("[note] build without USE_CUDA; this example shows the host-side reference path only.\n");
#endif

    pipeline_free_host(input);
    pipeline_free_host(serial_out);
    pipeline_free_host(async_out);
    return EXIT_SUCCESS;
}
