#include "export.h"
#include "queue.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
                break;
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
                    break;
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

        // Export option
        int export_choice;
        int export_status = safe_input_int(&export_choice,
                                           "\nDo you want to export this Circular Queue to a file? "
                                           "(1 for Yes, 2 for No, or '-1' to exit): ",
                                           1, 2);
        if (export_status != INPUT_EXIT_SIGNAL && export_status != 0 && export_choice == 1)
        {
            char path[256];
            int path_status =
                safe_input_string(path, "Enter filename to export (e.g. queue.json): ");
            if (path_status != INPUT_EXIT_SIGNAL && strlen(path) > 0)
            {
                int format_val;
                int format_status = safe_input_int(
                    &format_val,
                    "Select format (1 for Text, 2 for CSV, 3 for JSON, or '-1' to exit): ", 1, 3);
                if (format_status != INPUT_EXIT_SIGNAL && format_status != 0)
                {
                    ExportFormat format = EXPORT_FORMAT_TEXT;
                    if (format_val == 2)
                        format = EXPORT_FORMAT_CSV;
                    else if (format_val == 3)
                        format = EXPORT_FORMAT_JSON;

                    if (queue_export(&rollnos, QUEUE_TYPE_CIRCULAR, path, format, write_data_int))
                    {
                        printf("Exported successfully to %s\n", path);
                    }
                    else
                    {
                        printf("Failed to export to %s\n", path);
                    }
                }
            }
        }

        printf("\nExiting circular queue demo\n");
        destroy_circ_queue(&rollnos);
        return;
    }
}
