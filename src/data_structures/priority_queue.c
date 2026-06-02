#include "data_structures.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

char* return_heap_type(int heapType)
{
    return heapType == 0 ? "Min" : "Max";
}

priority_queue* pq_init(HeapType heapType)
{
    priority_queue* pq = malloc(sizeof(priority_queue));

    if (pq == NULL)
        return NULL;
    pq->size = 0;
    pq->heapType = heapType;

    return pq;
}

int insert(priority_queue* pq, int val)
{
    if (pq == NULL || pq->size == HEAP_CAPACITY)
        return 0;

    int i = pq->size;
    pq->heap[i] = val;
    pq->size++;

    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (pq->heapType == MIN_HEAP)
        {
            if (pq->heap[i] >= pq->heap[parent])
                break;
        }
        else
        {
            if (pq->heap[i] <= pq->heap[parent])
                break;
        }
        swap(&pq->heap[i], &pq->heap[parent]);
        i = parent;
    }

    return 1;
}

bool extractTop(priority_queue* pq, int* result)
{
    if (pq == NULL || result == NULL || pq->size == 0)
        return false;

    int topIndex = 0;
    int topElement = pq->heap[topIndex];
    int lastElementIndex = pq->size - 1;

    pq->heap[topIndex] = pq->heap[lastElementIndex];
    pq->size--;

    int i = 0;

    while (1)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int target = i;

        if (pq->heapType == MIN_HEAP)
        {
            if (left < pq->size && pq->heap[left] < pq->heap[target])
                target = left;

            if (right < pq->size && pq->heap[right] < pq->heap[target])
                target = right;
        }
        else
        {
            if (left < pq->size && pq->heap[left] > pq->heap[target])
                target = left;

            if (right < pq->size && pq->heap[right] > pq->heap[target])
                target = right;
        }

        if (target == i)
            break;

        swap(&pq->heap[i], &pq->heap[target]);
        i = target;
    }

    *result = topElement;

    return true;
}

bool peek_pq(priority_queue* pq, int* result)
{
    if (pq == NULL || result == NULL || pq->size == 0)
        return false;
    *result = pq->heap[0];
    return true;
}

void destroy_pq(priority_queue* pq)
{
    if (pq == NULL)
        return;

    free(pq);
}

void display_heap(priority_queue* pq)
{
    printf("\nHeap: ");
    print_array(pq->heap, pq->size);
    printf("\n");
}

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
                                                  1, 3);

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

                display_heap(pq);
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
