#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

int init_deque(int N, Queue* dq)
{
    if (N < 1)
        return 0;
    dq->arr = malloc(sizeof(void*) * N);
    if (dq->arr == NULL)
        return 0;
    dq->N = N;
    dq->front = -1;
    dq->rear = -1;
    return 1;
}

void destroy_deque(Queue* dq)
{
    if (dq == NULL || dq->arr == NULL)
        return;

    if (!deque_is_empty(dq))
    {
        int i = dq->front;

        while (1)
        {
            free(dq->arr[i]);

            if (i == dq->rear)
                break;

            i = (i + 1) % dq->N;
        }
    }

    free(dq->arr);
    dq->arr = NULL;
    dq->front = -1;
    dq->rear = -1;
    dq->N = 0;
}

bool deque_is_empty(const Queue* dq)
{
    return (dq == NULL || dq->front == -1);
}

bool deque_is_full(const Queue* dq)
{
    if (dq == NULL)
        return false;
    return ((dq->front == 0 && dq->rear == dq->N - 1) || (dq->front == dq->rear + 1));
}

int deque_insert_front(Queue* dq, void* value)
{
    if (dq == NULL || dq->arr == NULL || deque_is_full(dq))
        return -1;

    if (dq->front == -1) // empty
    {
        dq->front = 0;
        dq->rear = 0;
    }
    else if (dq->front == 0)
    {
        dq->front = dq->N - 1;
    }
    else
    {
        dq->front = dq->front - 1;
    }

    dq->arr[dq->front] = value;
    return 1;
}

int deque_insert_rear(Queue* dq, void* value)
{
    if (dq == NULL || dq->arr == NULL || deque_is_full(dq))
        return -1;

    if (dq->front == -1) // empty
    {
        dq->front = 0;
        dq->rear = 0;
    }
    else if (dq->rear == dq->N - 1)
    {
        dq->rear = 0;
    }
    else
    {
        dq->rear = dq->rear + 1;
    }

    dq->arr[dq->rear] = value;
    return 1;
}

void* deque_delete_front(Queue* dq)
{
    if (dq == NULL || dq->arr == NULL || deque_is_empty(dq))
        return NULL;

    void* removed = dq->arr[dq->front];

    if (dq->front == dq->rear) // only one element
    {
        dq->front = -1;
        dq->rear = -1;
    }
    else if (dq->front == dq->N - 1)
    {
        dq->front = 0;
    }
    else
    {
        dq->front = dq->front + 1;
    }

    return removed;
}

void* deque_delete_rear(Queue* dq)
{
    if (dq == NULL || dq->arr == NULL || deque_is_empty(dq))
        return NULL;

    void* removed = dq->arr[dq->rear];

    if (dq->front == dq->rear) // only one element
    {
        dq->front = -1;
        dq->rear = -1;
    }
    else if (dq->rear == 0)
    {
        dq->rear = dq->N - 1;
    }
    else
    {
        dq->rear = dq->rear - 1;
    }

    return removed;
}

int deque_get_front(const Queue* dq)
{
    if (dq == NULL || dq->arr == NULL || deque_is_empty(dq))
        return -1;
    return *(int*)dq->arr[dq->front];
}

int deque_get_rear(const Queue* dq)
{
    if (dq == NULL || dq->arr == NULL || deque_is_empty(dq))
        return -1;
    return *(int*)dq->arr[dq->rear];
}

void display_deque(const Queue* dq)
{
    if (dq == NULL || dq->arr == NULL || deque_is_empty(dq))
    {
        printf("\nDeque is empty\n");
        return;
    }
    printf("\nDeque elements: ");
    int i = dq->front;
    while (1)
    {
        printf("%d", *(int*)dq->arr[i]);
        if (i == dq->rear)
            break;
        printf("<->");
        i = (i + 1) % dq->N;
    }
    printf("\n");
}
