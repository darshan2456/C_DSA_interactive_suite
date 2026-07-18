#include "export.h"
#include "queue.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
                break;
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
                    break;
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

        // Export option
        int export_choice;
        int export_status = safe_input_int(&export_choice,
                                           "\nDo you want to export this Simple Queue to a file? "
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

                    if (queue_export(&q, QUEUE_TYPE_SIMPLE, path, format, write_data_int))
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

        printf("\nExiting simple queue demo\n");
        destroy_simple_queue(&q);
        return;
    }
}
