#include "queue.h"
#include "safe_input.h"
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

void simple_queue_demo(void)
{

    while (1)
    {
        Queue q = {0};
        int queue_capacity_value;
        int queue_capacity_status =
            safe_input_int(&queue_capacity_value,
                           "\n\nenter capacity number (N) of simple (linear) queue, (between 1 and "
                           "100), enter -1 to exit:- ",
                           1, 100);

        if (queue_capacity_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting simple queue demo\n");
            return;
        }
        if (queue_capacity_status == 0)
        {
            continue;
        }
        if (!init_simple_queue(queue_capacity_value, &q))
        {
            printf("\nmalloc allocation failure");
            return;
        }

        printf("\nNote: this is a LINEAR queue. rear only moves forward, so once it reaches the\n"
               "last slot the queue reports full even if dequeues freed slots at the front (the\n"
               "freed space is NOT reused). Compare this with the circular queue, which wraps\n"
               "around to reuse freed slots.\n");

        // loop of enqueue and dequeue (interleaved enqueue and dequeue)

        while (1)
        {
            int simple_queue_choice;
            int simple_queue_status =
                safe_input_int(&simple_queue_choice,
                               "\n\nenter 1 for enqueue, 2 for dequeue and -1 for exit:- ", 1, 2);

            if (simple_queue_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting simple queue demo\n");
                destroy_simple_queue(&q);
                return;
            }

            if (simple_queue_status == 0)
            { // invalid input, (chars or number+chars) loopback to start
                continue;
            }

            if (simple_queue_choice == 1)
            {
                int enqueue_val;
                int enqueue_val_status = safe_input_int(
                    &enqueue_val,
                    "\nEnter value to enqueue (between 1 and 100), enter -1 to exit:- ", 1, 100);

                if (enqueue_val_status == INPUT_EXIT_SIGNAL)
                {
                    printf("\nExiting simple queue demo\n");
                    destroy_simple_queue(&q);
                    return;
                }

                if (enqueue_val_status == 0)
                { // invalid input, (chars or number+chars) loopback to start
                    continue;
                }

                int* ptr = malloc(sizeof(int));
                if (ptr == NULL)
                {
                    printf("Malloc failed\n");
                    continue;
                }
                *ptr = enqueue_val;

                if (enqueue_simple(&q, ptr) == -1)
                {
                    free(ptr);
                    printf("\nQueue is full (linear overflow - freed front slots cannot be "
                           "reused)\n");
                }

                display_simple_queue(&q);
            }

            else if (simple_queue_choice == 2)
            {
                void* removed = dequeue_simple(&q);

                if (removed == NULL)
                {
                    printf("\nQueue is empty\n");
                }
                else
                {
                    printf("\nDequeued element: %d\n", *(int*)removed);
                    free(removed);
                }

                display_simple_queue(&q);
            }
        }
    }
}

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
