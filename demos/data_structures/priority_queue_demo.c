#include "priority_queue.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void priority_queue_demo(void)
{
    while (1)
    {
        int max_min_heap_val;
        int max_min_heap_status =
            safe_input_int(&max_min_heap_val, "\nEnter 0 for min heap or 1 for max heap: ", 0, 1);

        if (max_min_heap_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting priority queue demo.....\n");
            return;
        }

        if (max_min_heap_status == 0)
            continue;

        priority_queue* pq = pq_init((HeapType)max_min_heap_val);
        if (pq == NULL)
        {
            printf("\nAllocation failed, Exiting demo.\n");
            return;
        }

        while (1)
        {
            int pq_choice;
            int pq_choice_status = safe_input_int(&pq_choice,
                                                  "\nEnter 1 to insert, 2 to remove and extract "
                                                  "top, 3 to peek at top, -1 to exit demo: ",
                                                  -1, 3);

            if (pq_choice_status == INPUT_EXIT_SIGNAL)
            {
                destroy_pq(pq);
                printf("\nExiting priority queue demo.....\n");
                return;
            }

            if (pq_choice_status == 0)
                continue;

            if (pq_choice == 1)
            {
                int insert_val;
                int insert_status = safe_input_int(
                    &insert_val,
                    "\nEnter a number to insert into heap (Insert any whole number): ", 0, INT_MAX);

                if (insert_status == INPUT_EXIT_SIGNAL)
                {
                    destroy_pq(pq);
                    printf("\nExiting priority queue demo.....\n");
                    return;
                }

                if (insert_status == 0)
                    continue;

                if (insert(pq, insert_val) == 0)
                {
                    printf("\nHeap is full.\n");
                    continue;
                }
            }
            else if (pq_choice == 2)
            {
                int extracted_element;
                bool extracted_element_status = extractTop(pq, &extracted_element);

                if (!extracted_element_status)
                {
                    printf("\nHeap is empty.\n");
                    continue;
                }

                printf("\n%s element extracted: %d\n", return_heap_type((int)pq->heapType),
                       extracted_element);
                display_heap(pq);
            }
            else if (pq_choice == 3)
            {
                int peek_element;
                bool peek_element_status = peek_pq(pq, &peek_element);

                if (!peek_element_status)
                {
                    printf("\nHeap is empty.\n");
                    continue;
                }

                printf("\n%s element in heap: %d\n", return_heap_type((int)pq->heapType),
                       peek_element);
            }
        }
    }
}
