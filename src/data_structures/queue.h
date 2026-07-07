#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>

// Universal non-speacial queue structure
typedef struct Queue
{
    int rear;
    int front;
    int N;
    void** arr;
} Queue;

// Initialize a circular queue. Returns 1 on success, -1 on failure.
int init_circ_queue(int N, Queue* queue_ptr);

// Destroy a circular queue and free its resources.
void destroy_circ_queue(Queue* queue_ptr);

// Enqueue a value into a circular queue. Returns 1 on success, -1 if the queue is full.
int enqueue(Queue* queue_ptr, void* value);

// Dequeue a value from a circular queue. Returns Pointer to the dequeued value, or NULL if empty.
void* dequeue(Queue* queue_ptr);

// Display the contents of a circular queue.
void display_circ_queue(Queue* queue_ptr);

// Run the circular queue demonstration module.
void circular_queue_demo(void);

// Initialize a simple linear queue. Returns 1 on success, -1 on failure.
int init_simple_queue(int N, Queue* queue_ptr);

// Destroy a simple linear queue and free its resources.
void destroy_simple_queue(Queue* queue_ptr);

// Enqueue a value into a simple linear queue. Returns 1 on success, -1 if the queue is full.
int enqueue_simple(Queue* queue_ptr, void* value);

// Dequeue a value from a simple linear queue. Returns Pointer to the dequeued value, or NULL if
// empty.
void* dequeue_simple(Queue* queue_ptr);

// Display the contents of a simple linear queue.
void display_simple_queue(const Queue* queue_ptr);

// Run the simple queue demonstration module.
void simple_queue_demo(void);

// Initialize a double-ended queue. Returns 1 on success, -1 on failure.
int init_deque(int N, Queue* dq);

// Destroy a deque and free its resources.
void destroy_deque(Queue* dq);

// Insert a value at the front of a deque. Returns 1 on success, -1 if the deque is full.
int deque_insert_front(Queue* dq, void* value);

// Insert a value at the rear of a deque. Returns 1 on success, -1 if the deque is full.
int deque_insert_rear(Queue* dq, void* value);

// Delete and return the front value from a deque. Returns Pointer to the removed value, or NULL if
// empty.
void* deque_delete_front(Queue* dq);

// Delete and return the rear value from a deque. Returns Pointer to the removed value, or NULL if
// empty.
void* deque_delete_rear(Queue* dq);

// Get the front value from a deque without removing it. Returns The front value.
int deque_get_front(const Queue* dq);

// Get the rear value from a deque without removing it. Returns The rear value.
int deque_get_rear(const Queue* dq);

// Check whether a deque is empty. Returns true if empty, false otherwise.
bool deque_is_empty(const Queue* dq);

// Check whether a deque is full. Returns true if full, false otherwise.
bool deque_is_full(const Queue* dq);

// Display the contents of a deque.
void display_deque(const Queue* dq);

// Run the deque demonstration module.
void deque_demo(void);

#endif
