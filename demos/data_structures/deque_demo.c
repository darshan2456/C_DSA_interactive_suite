#include "queue.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

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
                                               "\nenter choice (\'-1\' to exit, or \'help\') : - ",
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
                if (ptr == NULL)
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
                if (ptr == NULL)
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
