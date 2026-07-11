#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdbool.h>

#define HEAP_CAPACITY 100

typedef enum
{
    MIN_HEAP = 0,
    MAX_HEAP = 1
} HeapType;

typedef struct priority_queue
{
    int size;
    HeapType heapType;
    int heap[HEAP_CAPACITY];
} priority_queue;

// Allocate and initialize a priority queue. Returns Pointer to the created priority queue, or NULL
// on failure.
priority_queue* pq_init(HeapType heapType);

// Insert a value into a priority queue. Returns 1 on success, -1 on failure.
int insert(priority_queue* pq, int val);

// Remove the top value from a priority queue. Returns true if a value was extracted, false if the
// queue is empty.
bool extractTop(priority_queue* pq, int* result);

// Peek at the top value of a priority queue without removing it. Returns true if the queue is not
// empty, false otherwise.
bool peek_pq(priority_queue* pq, int* result);

// Free resources used by a priority queue.
void destroy_pq(priority_queue* pq);

// Display the contents of a priority queue.
void display_heap(priority_queue* pq);

// Get string representation of heap type.
char* return_heap_type(int heapType);

// Run the priority queue demonstration module.
void priority_queue_demo(void);

#endif
