#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#include "../common/bench.h"
#include "../common/verify.h"
#include "blur.h"

static void fill_image(float *img, int width, int height) {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float fx = (float)x / (float)(width > 1 ? width - 1 : 1);
            float fy = (float)y / (float)(height > 1 ? height - 1 : 1);
            img[y * width + x] = 0.6f * fx + 0.4f * fy + 0.1f * sinf(8.0f * fx) * cosf(6.0f * fy);
        }
    }
}

static void usage(const char *argv0) {
    fprintf(stderr,
            "Usage: %s [width] [height] [threads]\n"
            "  width   : image width (default 1024)\n"
            "  height  : image height (default 768)\n"
            "  threads : OpenMP threads (default 4)\n",
            argv0);
}

int main(int argc, char **argv) {
    int width = (argc > 1) ? atoi(argv[1]) : 1024;
    int height = (argc > 2) ? atoi(argv[2]) : 768;
    int threads = (argc > 3) ? atoi(argv[3]) : 4;

    if (width <= 0 || height <= 0 || threads <= 0) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    printf("=== Chapter 5: OpenMP Blur ===\n");
    printf("width=%d height=%d threads=%d\n", width, height, threads);

    size_t n = (size_t)width * (size_t)height;
    float *input = (float *)malloc(n * sizeof(float));
    float *serial_out = (float *)malloc(n * sizeof(float));
    float *omp_out = (float *)malloc(n * sizeof(float));
    if (!input || !serial_out || !omp_out) {
        fprintf(stderr, "out of memory\n");
        return EXIT_FAILURE;
    }

    fill_image(input, width, height);

    omp_set_num_threads(threads);

    blur_ctx_t serial_ctx = {
        .width = width,
        .height = height,
        .input = input,
        .output = serial_out,
    };
    blur_ctx_t omp_ctx = serial_ctx;
    omp_ctx.output = omp_out;

    bench_result_t r_serial = bench_run("blur_serial", 3, 5, blur_serial, &serial_ctx);
    bench_result_t r_omp = bench_run("blur_openmp", 3, 5, blur_openmp, &omp_ctx);

    bench_report(&r_serial);
    bench_report(&r_omp);
    bench_speedup(&r_serial, &r_omp);

    if (verify_array_f32(omp_out, serial_out, (int)n, 1e-5f, 1e-5f) != 0) {
        free(input);
        free(serial_out);
        free(omp_out);
        return EXIT_FAILURE;
    }

    free(input);
    free(serial_out);
    free(omp_out);
    return EXIT_SUCCESS;
}
