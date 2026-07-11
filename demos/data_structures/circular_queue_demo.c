#include "queue.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

void circular_queue_demo(void)
{
    while (1)
    {
        Queue rollnos = {0};
        int queue_capacity_value;
        int queue_capacity_status =
            safe_input_int(&queue_capacity_value,
                           "\n\nenter capacity number (N) of circular queue, (between 2 and 100), "
                           "enter '-1' to exit:- ",
                           2, 100);

        if (queue_capacity_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting circular queue demo\n");
            destroy_circ_queue(&rollnos);
            return;
        }
        if (queue_capacity_status == 0)
        {
            continue;
        }
        if (!init_circ_queue(queue_capacity_value, &rollnos))
        {
            printf("\nmalloc allocation failure");
            return;
        }

        // loop of enqueue and dequeue (interleaved enqueue and dequeue)

        while (1)
        {
            int circ_queue_choice;
            int circ_queue_status =
                safe_input_int(&circ_queue_choice,
                               "\n\nenter 1 for enqueue, 2 for dequeue and '-1' for exit:- ", 1, 2);

            if (circ_queue_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting circular queue demo\n");
                destroy_circ_queue(&rollnos);
                return;
            }

            if (circ_queue_status == 0)
            { // invalid input, (chars or number+chars) loopback to start
                continue;
            }

            if (circ_queue_choice == 1)
            {
                int enqueue_val;
                int enqueue_val_status = safe_input_int(
                    &enqueue_val,
                    "\nEnter value to enqueue (between 1 and 100), enter '-1' to exit:- ", 1, 100);

                if (enqueue_val_status == INPUT_EXIT_SIGNAL)
                {
                    printf("\nExiting circular queue demo\n");
                    destroy_circ_queue(&rollnos);
                    return;
                }

                if (enqueue_val_status == 0)
                { // invalid input, (chars or number+chars) loopback to start
                    continue;
                }

                int* value = malloc(sizeof(int));
                if (value == NULL)
                {
                    printf("malloc failed\n");
                    continue;
                }
                *value = enqueue_val;
                if (enqueue(&rollnos, value) == -1)
                {
                    free(value);
                    printf("Queue is full (Circular Overflow)\n");
                }

                display_circ_queue(&rollnos);
            }

            else if (circ_queue_choice == 2)
            {
                void* removed = dequeue(&rollnos);

                if (removed == NULL)
                {
                    printf("\nQueue is empty\n");
                }
                else
                {
                    printf("\nDequeued element: %d\n", *(int*)removed);
                    free(removed);
                }

                display_circ_queue(&rollnos);
            }
        }
    }
}
