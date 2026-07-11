#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

// init function returns 0 on bad capacity / malloc failure and 1 on correct initialization

// enqueue_simple returns 1 on successful operation and -1 on failure due to a full queue

// dequeue_simple returns -1 when the queue is empty and the dequeued value on success

// This is a LINEAR queue: front and rear are plain array indices (both -1 when empty) and
// rear only ever advances. It does NOT wrap around, so once rear reaches the last slot the
// queue is reported full even if dequeues have freed slots at the front - that freed space
// is never reused. This "false overflow" is exactly the limitation the circular queue avoids
// by wrapping front/rear modulo N; the two implementations sit side-by-side for comparison.

int init_simple_queue(int N, Queue* queue_ptr)
{
    if (N < 1)
        return 0;
    queue_ptr->arr = malloc(sizeof(void*) * N);
    if (queue_ptr->arr == NULL)
        return 0;
    queue_ptr->N = N;
    queue_ptr->front = -1;
    queue_ptr->rear = -1;
    return 1;
}

void destroy_simple_queue(Queue* queue_ptr)
{
    if (queue_ptr == NULL || queue_ptr->arr == NULL)
        return;

    if (queue_ptr->front != -1)
    {
        for (int i = queue_ptr->front; i <= queue_ptr->rear; i++)
        {
            free(queue_ptr->arr[i]);
        }
    }

    free(queue_ptr->arr);
    queue_ptr->arr = NULL;
    queue_ptr->front = -1;
    queue_ptr->rear = -1;
    queue_ptr->N = 0;
}

int enqueue_simple(Queue* queue_ptr, void* value)
{
    // rear never moves backward, so a slot freed by dequeue at the front is not reclaimed
    if (queue_ptr == NULL || queue_ptr->arr == NULL || queue_ptr->rear == queue_ptr->N - 1)
        return -1;
    if (queue_ptr->front == -1)
        queue_ptr->front = 0;
    queue_ptr->rear++;
    queue_ptr->arr[queue_ptr->rear] = value;
    return 1;
}

void* dequeue_simple(Queue* queue_ptr)
{
    if (queue_ptr == NULL || queue_ptr->arr == NULL || queue_ptr->front == -1 ||
        queue_ptr->front > queue_ptr->rear)
        return NULL;
    void* front_value = queue_ptr->arr[queue_ptr->front];
    if (queue_ptr->front == queue_ptr->rear)
    {
        queue_ptr->front = -1;
        queue_ptr->rear = -1;
    }
    else
    {
        queue_ptr->front++;
    }
    return front_value;
}

void display_simple_queue(const Queue* queue_ptr)
{
    if (queue_ptr == NULL || queue_ptr->arr == NULL || queue_ptr->front == -1 ||
        queue_ptr->front > queue_ptr->rear)
    {
        printf("\nQueue (front -> rear): [empty]\n");
        return;
    }
    printf("\nQueue (front -> rear): ");
    for (int i = queue_ptr->front; i <= queue_ptr->rear; i++)
    {
        printf("%d -> ", *(int*)queue_ptr->arr[i]);
    }
    printf("END\n");
}
