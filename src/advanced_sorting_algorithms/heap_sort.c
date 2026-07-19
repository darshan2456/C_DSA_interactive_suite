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
