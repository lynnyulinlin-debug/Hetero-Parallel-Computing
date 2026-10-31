#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../common/bench.h"
#include "../common/verify.h"
#include "layernorm.h"

static void fill_matrix(float *x, int rows, int cols) {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            float base = 0.001f * (float)(r * cols + c);
            x[(size_t)r * (size_t)cols + c] =
                sinf(0.013f * base) + 0.5f * cosf(0.007f * (float)c) + 0.01f * (float)(r % 11);
        }
    }
}

static void usage(const char *argv0) {
    fprintf(stderr,
            "Usage: %s [rows] [cols]\n"
            "  rows : matrix rows (default 256)\n"
            "  cols : matrix cols (default 4096)\n",
            argv0);
}

int main(int argc, char **argv) {
    int rows = (argc > 1) ? atoi(argv[1]) : 256;
    int cols = (argc > 2) ? atoi(argv[2]) : 4096;

    if (rows <= 0 || cols <= 0) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    printf("=== Chapter 6: SIMD LayerNorm ===\n");
    printf("rows=%d cols=%d\n", rows, cols);

    size_t n = (size_t)rows * (size_t)cols;
    float *input = (float *)malloc(n * sizeof(float));
    float *serial_out = (float *)malloc(n * sizeof(float));
    float *simd_out = (float *)malloc(n * sizeof(float));
    if (!input || !serial_out || !simd_out) {
        fprintf(stderr, "out of memory\n");
        return EXIT_FAILURE;
    }

    fill_matrix(input, rows, cols);

    layernorm_ctx_t serial_ctx = {
        .rows = rows,
        .cols = cols,
        .eps = 1e-5f,
        .input = input,
        .output = serial_out,
    };
    layernorm_ctx_t simd_ctx = serial_ctx;
    simd_ctx.output = simd_out;

    bench_result_t r_serial = bench_run("layernorm_serial", 3, 5, layernorm_serial, &serial_ctx);
    bench_result_t r_simd = bench_run("layernorm_simd", 3, 5, layernorm_simd, &simd_ctx);

    bench_report(&r_serial);
    bench_report(&r_simd);
    bench_speedup(&r_serial, &r_simd);

    if (verify_array_f32(simd_out, serial_out, (int)n, 1e-5f, 1e-5f) != 0) {
        free(input);
        free(serial_out);
        free(simd_out);
        return EXIT_FAILURE;
    }

    free(input);
    free(serial_out);
    free(simd_out);
    return EXIT_SUCCESS;
}
