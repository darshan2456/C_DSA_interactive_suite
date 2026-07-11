#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdbool.h>
#include <stddef.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <pthread.h>
#endif

typedef enum {
    RB_SUCCESS = 0,
    RB_ERROR_NULL_POINTER = -1,
    RB_ERROR_FULL = -2,
    RB_ERROR_EMPTY = -3,
    RB_ERROR_MEMORY = -4
} rb_status_t;

typedef struct {
    void **buffer;
    size_t capacity;
    size_t count;
    size_t head;
    size_t tail;

#if defined(_WIN32)
    CRITICAL_SECTION mutex;
    CONDITION_VARIABLE not_empty;
    CONDITION_VARIABLE not_full;
#else
    pthread_mutex_t mutex;
    pthread_cond_t not_empty;
    pthread_cond_t not_full;
#endif

} ring_buffer_t;

/**
 * Creates a new ring buffer with the specified capacity.
 * @param capacity The maximum number of elements the buffer can hold.
 * @return A pointer to the newly allocated ring buffer, or NULL on failure.
 */
ring_buffer_t* rb_create(size_t capacity);

/**
 * Destroys the ring buffer and frees associated resources.
 * @param rb A pointer to the ring buffer.
 */
void rb_destroy(ring_buffer_t *rb);

/**
 * Enqueues an item into the ring buffer (non-blocking).
 * @param rb A pointer to the ring buffer.
 * @param data A pointer to the data to enqueue.
 * @return RB_SUCCESS on success, RB_ERROR_FULL if buffer is full, etc.
 */
rb_status_t rb_enqueue(ring_buffer_t *rb, void *data);

/**
 * Dequeues an item from the ring buffer (non-blocking).
 * @param rb A pointer to the ring buffer.
 * @param data A pointer to a pointer where the dequeued data will be stored.
 * @return RB_SUCCESS on success, RB_ERROR_EMPTY if buffer is empty, etc.
 */
rb_status_t rb_dequeue(ring_buffer_t *rb, void **data);

/**
 * Enqueues an item into the ring buffer, blocking if the buffer is full.
 * @param rb A pointer to the ring buffer.
 * @param data A pointer to the data to enqueue.
 * @return RB_SUCCESS on success, or an error code.
 */
rb_status_t rb_enqueue_blocking(ring_buffer_t *rb, void *data);

/**
 * Dequeues an item from the ring buffer, blocking if the buffer is empty.
 * @param rb A pointer to the ring buffer.
 * @param data A pointer to a pointer where the dequeued data will be stored.
 * @return RB_SUCCESS on success, or an error code.
 */
rb_status_t rb_dequeue_blocking(ring_buffer_t *rb, void **data);

/**
 * Returns the current number of elements in the ring buffer.
 * @param rb A pointer to the ring buffer.
 * @return The number of elements currently stored.
 */
size_t rb_get_count(ring_buffer_t *rb);

/**
 * Returns the capacity of the ring buffer.
 * @param rb A pointer to the ring buffer.
 * @return The maximum number of elements the buffer can hold.
 */
size_t rb_get_capacity(ring_buffer_t *rb);

/**
 * Checks if the ring buffer is empty.
 * @param rb A pointer to the ring buffer.
 * @return true if empty, false otherwise.
 */
bool rb_is_empty(ring_buffer_t *rb);

/**
 * Checks if the ring buffer is full.
 * @param rb A pointer to the ring buffer.
 * @return true if full, false otherwise.
 */
bool rb_is_full(ring_buffer_t *rb);

#endif // RING_BUFFER_H
