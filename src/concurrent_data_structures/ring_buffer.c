#include "ring_buffer.h"
#include <stdlib.h>
#include <stdio.h>

ring_buffer_t* rb_create(size_t capacity) {
    if (capacity == 0) return NULL;

    ring_buffer_t *rb = (ring_buffer_t*)malloc(sizeof(ring_buffer_t));
    if (!rb) return NULL;

    rb->buffer = (void**)malloc(sizeof(void*) * capacity);
    if (!rb->buffer) {
        free(rb);
        return NULL;
    }

    rb->capacity = capacity;
    rb->count = 0;
    rb->head = 0;
    rb->tail = 0;

#if defined(_WIN32)
    InitializeCriticalSection(&rb->mutex);
    InitializeConditionVariable(&rb->not_empty);
    InitializeConditionVariable(&rb->not_full);
#else
    if (pthread_mutex_init(&rb->mutex, NULL) != 0) {
        free(rb->buffer);
        free(rb);
        return NULL;
    }
    if (pthread_cond_init(&rb->not_empty, NULL) != 0) {
        pthread_mutex_destroy(&rb->mutex);
        free(rb->buffer);
        free(rb);
        return NULL;
    }
    if (pthread_cond_init(&rb->not_full, NULL) != 0) {
        pthread_cond_destroy(&rb->not_empty);
        pthread_mutex_destroy(&rb->mutex);
        free(rb->buffer);
        free(rb);
        return NULL;
    }
#endif

    return rb;
}

void rb_destroy(ring_buffer_t *rb) {
    if (!rb) return;

#if defined(_WIN32)
    DeleteCriticalSection(&rb->mutex);
    // Condition variables in Windows do not need to be destroyed
#else
    pthread_mutex_destroy(&rb->mutex);
    pthread_cond_destroy(&rb->not_empty);
    pthread_cond_destroy(&rb->not_full);
#endif

    free(rb->buffer);
    free(rb);
}

static void lock_mutex(ring_buffer_t *rb) {
#if defined(_WIN32)
    EnterCriticalSection(&rb->mutex);
#else
    pthread_mutex_lock(&rb->mutex);
#endif
}

static void unlock_mutex(ring_buffer_t *rb) {
#if defined(_WIN32)
    LeaveCriticalSection(&rb->mutex);
#else
    pthread_mutex_unlock(&rb->mutex);
#endif
}

static void wait_not_full(ring_buffer_t *rb) {
#if defined(_WIN32)
    SleepConditionVariableCS(&rb->not_full, &rb->mutex, INFINITE);
#else
    pthread_cond_wait(&rb->not_full, &rb->mutex);
#endif
}

static void wait_not_empty(ring_buffer_t *rb) {
#if defined(_WIN32)
    SleepConditionVariableCS(&rb->not_empty, &rb->mutex, INFINITE);
#else
    pthread_cond_wait(&rb->not_empty, &rb->mutex);
#endif
}

static void signal_not_full(ring_buffer_t *rb) {
#if defined(_WIN32)
    WakeConditionVariable(&rb->not_full);
#else
    pthread_cond_signal(&rb->not_full);
#endif
}

static void signal_not_empty(ring_buffer_t *rb) {
#if defined(_WIN32)
    WakeConditionVariable(&rb->not_empty);
#else
    pthread_cond_signal(&rb->not_empty);
#endif
}

rb_status_t rb_enqueue(ring_buffer_t *rb, void *data) {
    if (!rb) return RB_ERROR_NULL_POINTER;

    lock_mutex(rb);

    if (rb->count == rb->capacity) {
        unlock_mutex(rb);
        return RB_ERROR_FULL;
    }

    rb->buffer[rb->tail] = data;
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count++;

    signal_not_empty(rb);
    unlock_mutex(rb);

    return RB_SUCCESS;
}

rb_status_t rb_dequeue(ring_buffer_t *rb, void **data) {
    if (!rb || !data) return RB_ERROR_NULL_POINTER;

    lock_mutex(rb);

    if (rb->count == 0) {
        unlock_mutex(rb);
        return RB_ERROR_EMPTY;
    }

    *data = rb->buffer[rb->head];
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count--;

    signal_not_full(rb);
    unlock_mutex(rb);

    return RB_SUCCESS;
}

rb_status_t rb_enqueue_blocking(ring_buffer_t *rb, void *data) {
    if (!rb) return RB_ERROR_NULL_POINTER;

    lock_mutex(rb);

    while (rb->count == rb->capacity) {
        wait_not_full(rb);
    }

    rb->buffer[rb->tail] = data;
    rb->tail = (rb->tail + 1) % rb->capacity;
    rb->count++;

    signal_not_empty(rb);
    unlock_mutex(rb);

    return RB_SUCCESS;
}

rb_status_t rb_dequeue_blocking(ring_buffer_t *rb, void **data) {
    if (!rb || !data) return RB_ERROR_NULL_POINTER;

    lock_mutex(rb);

    while (rb->count == 0) {
        wait_not_empty(rb);
    }

    *data = rb->buffer[rb->head];
    rb->head = (rb->head + 1) % rb->capacity;
    rb->count--;

    signal_not_full(rb);
    unlock_mutex(rb);

    return RB_SUCCESS;
}

size_t rb_get_count(ring_buffer_t *rb) {
    if (!rb) return 0;
    lock_mutex(rb);
    size_t count = rb->count;
    unlock_mutex(rb);
    return count;
}

size_t rb_get_capacity(ring_buffer_t *rb) {
    if (!rb) return 0;
    // Capacity is constant after creation, no lock strictly needed, but let's be safe if it were to change in future
    lock_mutex(rb);
    size_t capacity = rb->capacity;
    unlock_mutex(rb);
    return capacity;
}

bool rb_is_empty(ring_buffer_t *rb) {
    if (!rb) return true;
    lock_mutex(rb);
    bool empty = (rb->count == 0);
    unlock_mutex(rb);
    return empty;
}

bool rb_is_full(ring_buffer_t *rb) {
    if (!rb) return false;
    lock_mutex(rb);
    bool full = (rb->count == rb->capacity);
    unlock_mutex(rb);
    return full;
}
