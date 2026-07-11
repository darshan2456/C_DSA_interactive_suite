#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

// init function returns -1 on malloc failure and 1 on correct initialization

// enqueue returns 1 on succesful operation and -1 on failure due to full circular queue

// dequeue returns -1 when circular queue is empty and value when operation is succesful

int init_circ_queue(int N, Queue* queue_ptr)
{
    if (N < 1)
        return 0;
    queue_ptr->arr = malloc(sizeof(void*) * N);
    if (queue_ptr->arr == NULL)
        return 0;
    queue_ptr->N = N;
    queue_ptr->rear = 0;
    queue_ptr->front = 0;
    return 1;
}

void destroy_circ_queue(Queue* queue_ptr)
{
    if (queue_ptr == NULL || queue_ptr->arr == NULL)
        return;

    int i = queue_ptr->front;

    while (i != queue_ptr->rear)
    {
        free(queue_ptr->arr[i]);
        i = (i + 1) % queue_ptr->N;
    }

    free(queue_ptr->arr);
    queue_ptr->arr = NULL;
    queue_ptr->front = 0;
    queue_ptr->rear = 0;
    queue_ptr->N = 0;
}

int enqueue(Queue* queue_ptr, void* value)
{
    // one slot is kept empty to differentiate between full and empty queue
    if ((queue_ptr == NULL) || (queue_ptr->arr == NULL) ||
        (((queue_ptr->rear) + 1) % (queue_ptr->N) == queue_ptr->front))
        return -1;

    queue_ptr->arr[queue_ptr->rear] = value;
    queue_ptr->rear = ((queue_ptr->rear) + 1) % (queue_ptr->N);
    return 1;
}

void* dequeue(Queue* queue_ptr)
{
    if (queue_ptr == NULL || queue_ptr->arr == NULL || queue_ptr->rear == queue_ptr->front)
        return NULL;
    int front_value = queue_ptr->front;
    queue_ptr->front = ((queue_ptr->front) + 1) % (queue_ptr->N);
    return queue_ptr->arr[front_value];
}

void display_circ_queue(Queue* queue_ptr)
{
    if (queue_ptr == NULL)
        return;

    int i = queue_ptr->front;
    while (i != queue_ptr->rear)
    {
        printf("%d<->", *(int*)queue_ptr->arr[i]);
        i = (i + 1) % queue_ptr->N;
    }
}