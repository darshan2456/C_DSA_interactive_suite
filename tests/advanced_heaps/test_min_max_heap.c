#include "advanced_heaps.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_min_max_heap_ops()
{
    MinMaxHeap* heap = create_min_max_heap(20);
    assert(heap != NULL);

    /* Insert keys: 5, 20, 3, 10, 15, 30, 2 */
    assert(min_max_heap_insert(heap, 5, 50));
    assert(min_max_heap_insert(heap, 20, 200));
    assert(min_max_heap_insert(heap, 3, 30));
    assert(min_max_heap_insert(heap, 10, 100));
    assert(min_max_heap_insert(heap, 15, 150));
    assert(min_max_heap_insert(heap, 30, 300));
    assert(min_max_heap_insert(heap, 2, 20));

    int min_k, min_v;
    int max_k, max_v;

    /* Max should be 30, Min should be 2 */
    assert(min_max_heap_get_max(heap, &max_k, &max_v));
    assert(max_k == 30);

    assert(min_max_heap_get_min(heap, &min_k, &min_v));
    assert(min_k == 2);

    /* Extract Min: 2 */
    assert(min_max_heap_extract_min(heap, &min_k, &min_v));
    assert(min_k == 2);

    /* Extract Max: 30 */
    assert(min_max_heap_extract_max(heap, &max_k, &max_v));
    assert(max_k == 30);

    /* Extract Min: 3 */
    assert(min_max_heap_extract_min(heap, &min_k, &min_v));
    assert(min_k == 3);

    /* Extract Max: 20 */
    assert(min_max_heap_extract_max(heap, &max_k, &max_v));
    assert(max_k == 20);

    destroy_min_max_heap(heap);
    printf("Min-Max Heap tests passed\n");
}

int main()
{
    test_min_max_heap_ops();

    printf("All Min-Max Heap tests passed\n");
    return 0;
}
