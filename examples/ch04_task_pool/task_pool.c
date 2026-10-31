#include "task_pool.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int begin;
    int end;
} task_t;

typedef struct {
    task_t *items;
    int cap;
    int head;
    int tail;
    int count;
    int closed;
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
} task_queue_t;

typedef struct {
    task_queue_t *queue;
    const task_pool_ctx_t *ctx;
} worker_state_t;

static int imin(int a, int b) {
    return a < b ? a : b;
}

static float task_step(float x, int repeat) {
    float y = x;
    for (int r = 0; r < repeat; ++r) {
        y = y * 1.0001f + 0.0001f * (float)(r + 1);
    }
    return y;
}

static void process_range(const task_pool_ctx_t *ctx, int begin, int end) {
    for (int i = begin; i < end; ++i) {
        ctx->output[i] = task_step(ctx->input[i], ctx->repeat);
    }
}

static void task_queue_init(task_queue_t *q, int cap) {
    q->items = (task_t *)calloc((size_t)cap, sizeof(task_t));
    q->cap = cap;
    q->head = q->tail = q->count = 0;
    q->closed = 0;
    pthread_mutex_init(&q->mutex, NULL);
    pthread_cond_init(&q->not_empty, NULL);
    pthread_cond_init(&q->not_full, NULL);
}

static void task_queue_destroy(task_queue_t *q) {
    free(q->items);
    pthread_mutex_destroy(&q->mutex);
    pthread_cond_destroy(&q->not_empty);
    pthread_cond_destroy(&q->not_full);
}

static void task_queue_close(task_queue_t *q) {
    pthread_mutex_lock(&q->mutex);
    q->closed = 1;
    pthread_cond_broadcast(&q->not_empty);
    pthread_cond_broadcast(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
}

static void task_queue_push(task_queue_t *q, task_t task) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == q->cap && !q->closed) {
        pthread_cond_wait(&q->not_full, &q->mutex);
    }
    if (!q->closed) {
        q->items[q->tail] = task;
        q->tail = (q->tail + 1) % q->cap;
        ++q->count;
        pthread_cond_signal(&q->not_empty);
    }
    pthread_mutex_unlock(&q->mutex);
}

static int task_queue_pop(task_queue_t *q, task_t *task) {
    pthread_mutex_lock(&q->mutex);
    while (q->count == 0 && !q->closed) {
        pthread_cond_wait(&q->not_empty, &q->mutex);
    }
    if (q->count == 0 && q->closed) {
        pthread_mutex_unlock(&q->mutex);
        return 0;
    }
    *task = q->items[q->head];
    q->head = (q->head + 1) % q->cap;
    --q->count;
    pthread_cond_signal(&q->not_full);
    pthread_mutex_unlock(&q->mutex);
    return 1;
}

static void *worker_main(void *arg) {
    worker_state_t *st = (worker_state_t *)arg;
    task_t task;
    while (task_queue_pop(st->queue, &task)) {
        process_range(st->ctx, task.begin, task.end);
    }
    return NULL;
}

void task_pool_serial(void *arg) {
    const task_pool_ctx_t *ctx = (const task_pool_ctx_t *)arg;
    int n = ctx->n;
    int chunk = ctx->chunk > 0 ? ctx->chunk : 1;
    for (int begin = 0; begin < n; begin += chunk) {
        int end = imin(begin + chunk, n);
        process_range(ctx, begin, end);
    }
}

void task_pool_threaded(void *arg) {
    const task_pool_ctx_t *ctx = (const task_pool_ctx_t *)arg;
    int n = ctx->n;
    int chunk = ctx->chunk > 0 ? ctx->chunk : 1;
    int workers = ctx->workers > 0 ? ctx->workers : 1;
    int cap = workers * 4 + 8;

    task_queue_t queue;
    task_queue_init(&queue, cap);

    pthread_t *threads = (pthread_t *)calloc((size_t)workers, sizeof(pthread_t));
    worker_state_t *states =
        (worker_state_t *)calloc((size_t)workers, sizeof(worker_state_t));

    for (int i = 0; i < workers; ++i) {
        states[i].queue = &queue;
        states[i].ctx = ctx;
        if (pthread_create(&threads[i], NULL, worker_main, &states[i]) != 0) {
            fprintf(stderr, "[task_pool] failed to create worker %d\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (int begin = 0; begin < n; begin += chunk) {
        task_t task = {.begin = begin, .end = imin(begin + chunk, n)};
        task_queue_push(&queue, task);
    }

    task_queue_close(&queue);

    for (int i = 0; i < workers; ++i) {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(states);
    task_queue_destroy(&queue);
}
