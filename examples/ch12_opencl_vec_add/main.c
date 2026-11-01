#include <stdio.h>
#include <stdlib.h>
#include "../common/verify.h"
#include "vec_add.h"

int main(int argc, char **argv) {
    int n = argc > 1 ? atoi(argv[1]) : 4099;
    if (n <= 0) return EXIT_FAILURE;
    float *a = malloc((size_t)n * sizeof(float)), *b = malloc((size_t)n * sizeof(float));
    float *ref = malloc((size_t)n * sizeof(float)), *out = malloc((size_t)n * sizeof(float));
    if (!a || !b || !ref || !out) return EXIT_FAILURE;
    for (int i = 0; i < n; ++i) { a[i] = (float)(i % 31); b[i] = (float)(i % 7); }
    vec_add_serial(a, b, ref, n);
    int status = vec_add_opencl(a, b, out, n);
    if (status == 0) status = verify_array_f32(out, ref, n, 0.f, 0.f);
    free(a); free(b); free(ref); free(out);
    return status < 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}
