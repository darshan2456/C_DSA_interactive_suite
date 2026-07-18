#include "advanced_sorting.h"
#include "array.h"
#include "priority_queue.h"
#include "safe_input.h"
#include "sorting_visualizer.h"
#include "telemetry.h"
#include <stdio.h>
#include <time.h>

/*
 * Heap sort implemented on top of the existing priority_queue (binary heap).
 *
 *   Build phase: insert all n elements into a MIN-heap   -> n * O(log n)
 *   Sort phase : extractTop n times                       -> n * O(log n)
 *                a min-heap always yields the smallest remaining element,
 *                so writing them back left-to-right gives ascending order.
 *
 * Overall: O(n log n). Not in-place (the heap keeps its own copy) and bounded
 * by HEAP_CAPACITY because priority_queue uses a fixed-size array.
 */
void heap_sort(int arr[], int n)
{
    if (n <= 1)
        return;

    telemetry_init("heap_sort");

    priority_queue* pq = pq_init(MIN_HEAP);
    if (pq == NULL)
    {
        telemetry_close();
        return;
    }

    int inserted = 0;
    for (int i = 0; i < n; i++)
    {
        visualize_sort(arr, n, i, -1, -1, "Heap Sort: Inserting elements into Min-Heap");
        if (insert(pq, arr[i]) == 0) // heap full (HEAP_CAPACITY reached)
            break;
        inserted++;
    }

    for (int i = 0; i < inserted; i++)
    {
        extractTop(pq, &arr[i]); // min first -> ascending order
        visualize_sort(arr, n, i, -1, -1, "Heap Sort: Extracting min elements back to array");
    }

    destroy_pq(pq);
    telemetry_close();
}

void heap_sort_demo(void)
{
    int length_of_array;
    while (1)
    {
        printf("\n\nHeap sort demo");
        int length_status = safe_input_int(&length_of_array,
                                           "\nenter the number of elements in the array "
                                           "(between 1 and 100), enter '-1' to exit:- ",
                                           1, 100);

        if (length_status == 0)
            continue; // retry on failure

        if (length_status == INPUT_EXIT_SIGNAL)
        { // user entered -1
            printf("\nExiting heap sort demo....\n");
            return;
        }

        int arr[length_of_array];

        for (int i = 0; i < length_of_array; i++)
        {

        retry:
            printf("\nenter the element number %d, (between 1 and 100), enter '-1' to exit:- ", i);
            int element_status = safe_input_int(&arr[i], NULL, 1, 100);

            if (element_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting heap sort demo.....\n");
                return;
            }

            if (element_status == 0)
            {
                goto retry;
            }
        }

        clock_t start_t = clock();
        heap_sort(arr, length_of_array);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("\nThe array sorted by heap sort is:- ");
        for (int i = 0; i < length_of_array; i++)
        {
            printf("%d", arr[i]);
            if (i < length_of_array - 1)
                printf(",");
        }
        printf("\nTotal CPU time taken:- %f seconds", total_t);
    }
}
