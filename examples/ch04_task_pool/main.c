#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/bench.h"
#include "../common/verify.h"
#include "task_pool.h"

static int auto_workers(void) {
    long ncpu = sysconf(_SC_NPROCESSORS_ONLN);
    if (ncpu < 1) return 1;
    return (int)(ncpu > 1 ? ncpu - 1 : 1);
}

static void fill_input(float *x, int n) {
    for (int i = 0; i < n; ++i) {
        x[i] = sinf(0.001f * (float)i) + 0.0001f * (float)(i % 17);
    }
}

static void usage(const char *argv0) {
    fprintf(stderr,
            "Usage: %s [n] [chunk] [workers] [repeat]\n"
            "  n       : number of elements (default 1048576)\n"
            "  chunk   : tasks per chunk (default 4096)\n"
            "  workers : worker threads (default auto)\n"
            "  repeat  : inner compute repeats (default 128)\n",
            argv0);
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : (1 << 20);
    int chunk = (argc > 2) ? atoi(argv[2]) : 4096;
    int workers = (argc > 3) ? atoi(argv[3]) : auto_workers();
    int repeat = (argc > 4) ? atoi(argv[4]) : 128;

    if (n <= 0 || chunk <= 0 || workers <= 0 || repeat <= 0) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    printf("=== Chapter 4: Task Pool ===\n");
    printf("n=%d chunk=%d workers=%d repeat=%d\n", n, chunk, workers, repeat);

    float *input = (float *)malloc((size_t)n * sizeof(float));
    float *serial_out = (float *)malloc((size_t)n * sizeof(float));
    float *threaded_out = (float *)malloc((size_t)n * sizeof(float));
    if (!input || !serial_out || !threaded_out) {
        fprintf(stderr, "out of memory\n");
        return EXIT_FAILURE;
    }

    fill_input(input, n);

    task_pool_ctx_t serial_ctx = {
        .n = n,
        .chunk = chunk,
        .workers = 1,
        .repeat = repeat,
        .input = input,
        .output = serial_out,
    };
    task_pool_ctx_t threaded_ctx = serial_ctx;
    threaded_ctx.workers = workers;
    threaded_ctx.output = threaded_out;

    bench_result_t r_serial = bench_run("task_pool_serial", 3, 5, task_pool_serial, &serial_ctx);
    bench_result_t r_threaded = bench_run("task_pool_threaded", 3, 5, task_pool_threaded, &threaded_ctx);

    bench_report(&r_serial);
    bench_report(&r_threaded);
    bench_speedup(&r_serial, &r_threaded);

    if (verify_array_f32(threaded_out, serial_out, n, 1e-5f, 1e-5f) != 0) {
        free(input);
        free(serial_out);
        free(threaded_out);
        return EXIT_FAILURE;
    }

    free(input);
    free(serial_out);
    free(threaded_out);
    return EXIT_SUCCESS;
}
