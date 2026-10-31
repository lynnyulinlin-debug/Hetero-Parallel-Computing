// attention_openmp.c —— 单头 causal attention 的 OpenMP 版本。
#include "attention.h"

#include <float.h>
#include <math.h>
#include <stddef.h>
#include <stdlib.h>

#include <omp.h>

void attention_openmp(const float *q, const float *k, const float *v, float *o,
                      int rows, int dim) {
    float inv_sqrt_dim = 1.0f / sqrtf((float)dim);

    #pragma omp parallel
    {
        float *scores = (float *)malloc(sizeof(float) * (size_t)rows);
        if (!scores) {
            abort();
        }

        #pragma omp for schedule(static)
        for (int i = 0; i < rows; ++i) {
            const float *qi = q + (size_t)i * (size_t)dim;

            float maxv = -FLT_MAX;
            for (int j = 0; j <= i; ++j) {
                const float *kj = k + (size_t)j * (size_t)dim;
                float dot = 0.0f;
                for (int d = 0; d < dim; ++d) {
                    dot += qi[d] * kj[d];
                }
                scores[j] = dot * inv_sqrt_dim;
                if (scores[j] > maxv) maxv = scores[j];
            }

            float sum = 0.0f;
            for (int j = 0; j <= i; ++j) {
                scores[j] = expf(scores[j] - maxv);
                sum += scores[j];
            }
            float inv_sum = 1.0f / sum;
            for (int j = 0; j <= i; ++j) {
                scores[j] *= inv_sum;
            }

            float *oi = o + (size_t)i * (size_t)dim;
            for (int d = 0; d < dim; ++d) {
                float acc = 0.0f;
                for (int j = 0; j <= i; ++j) {
                    const float *vj = v + (size_t)j * (size_t)dim;
                    acc += scores[j] * vj[d];
                }
                oi[d] = acc;
            }
        }

        free(scores);
    }
}
