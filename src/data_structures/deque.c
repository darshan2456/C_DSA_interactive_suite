#include "data_structures.h"
#include "safe_input.h"
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
    if(dq == NULL)
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

void deque_demo(void)
{
    while (1)
    {
        Queue dq = {0};
        int capacity;
        int status = safe_input_int(&capacity,
                                    "\n\nenter capacity number (N) of deque (between 1 and 100), "
                                    "enter '-1' to exit:- ",
                                    1, 100);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting deque demo\n");
            return;
        }
        if (status == 0)
        {
            continue;
        }
        if (!init_deque(capacity, &dq))
        {
            printf("\nmalloc allocation failure");
            return;
        }

        while (1)
        {
            int choice;
            int choice_status = safe_input_int(&choice,
                                               "\n1. Insert Front"
                                               "\n2. Insert Rear"
                                               "\n3. Delete Front"
                                               "\n4. Delete Rear"
                                               "\n5. Get Front"
                                               "\n6. Get Rear"
                                               "\nenter choice, '-1' to exit this deque:- ",
                                               1, 6);

            if (choice_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting deque demo\n");
                destroy_deque(&dq);
                return;
            }
            if (choice_status == 0)
            {
                continue;
            }

            if (choice == 1)
            {
                int val;
                int val_status = safe_input_int(
                    &val, "\nEnter value to insert at front (1 to 100), '-1' to exit: ", 1, 100);
                if (val_status == INPUT_EXIT_SIGNAL)
                {
                    printf("\nExiting deque demo\n");
                    destroy_deque(&dq);
                    return;
                }
                if (val_status == 0)
                    continue;
                
                int* ptr = malloc(sizeof(int));
                if(ptr == NULL)
                {
                    printf("Malloc failed\n");
                    continue;
                }
                *ptr = val;

                if (deque_insert_front(&dq, ptr) == -1)
                {
                    free(ptr);
                    printf("\nDeque is full (overflow)\n");
                }
                display_deque(&dq);
            }
            else if (choice == 2)
            {
                int val;
                int val_status = safe_input_int(
                    &val, "\nEnter value to insert at rear (1 to 100), '-1' to exit: ", 1, 100);
                if (val_status == INPUT_EXIT_SIGNAL)
                {
                    printf("\nExiting deque demo\n");
                    destroy_deque(&dq);
                    return;
                }
                if (val_status == 0)
                    continue;
                
                int* ptr = malloc(sizeof(int));
                if(ptr == NULL)
                {
                    printf("Malloc failed\n");
                    continue;
                }
                *ptr = val;

                if (deque_insert_rear(&dq, ptr) == -1)
                {
                    free(ptr);
                    printf("\nDeque is full (overflow)\n");
                }
                display_deque(&dq);
            }
            else if (choice == 3)
            {
                void* removed_val = deque_delete_front(&dq);
                if (removed_val == NULL)
                {
                    printf("\nDeque is empty (underflow)\n");
                }
                else
                {
                    printf("\nDeleted element from front: %d\n", *(int*)removed_val);
                    free(removed_val);
                }
                display_deque(&dq);
            }
            else if (choice == 4)
            {
                void* removed_val = deque_delete_rear(&dq);
                if (removed_val == NULL)
                {
                    printf("\nDeque is empty (underflow)\n");
                }
                else
                {
                    printf("\nDeleted element from rear: %d\n", *(int*)removed_val);
                    free(removed_val);
                }
                display_deque(&dq);
            }
            else if (choice == 5)
            {
                int val = deque_get_front(&dq);
                if (val == -1)
                {
                    printf("\nDeque is empty\n");
                }
                else
                {
                    printf("\nFront element: %d\n", val);
                }
            }
            else if (choice == 6)
            {
                int val = deque_get_rear(&dq);
                if (val == -1)
                {
                    printf("\nDeque is empty\n");
                }
                else
                {
                    printf("\nRear element: %d\n", val);
                }
            }
        }
    }
}
