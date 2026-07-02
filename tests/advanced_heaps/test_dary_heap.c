#include "advanced_heaps.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_dary_insert_and_extract()
{
    printf("Running test_dary_insert_and_extract...\n");

    // Create a 3-ary heap
    DAryHeap* heap = create_dary_heap(10, 3);
    assert(heap != NULL);
    assert(heap->d == 3);
    assert(heap->size == 0);

    assert(dary_heap_insert(heap, 15, 150));
    assert(dary_heap_insert(heap, 10, 100));
    assert(dary_heap_insert(heap, 30, 300));
    assert(dary_heap_insert(heap, 5, 50));
    assert(dary_heap_insert(heap, 20, 200));

    assert(heap->size == 5);

    int key, val;
    assert(dary_heap_get_min(heap, &key, &val));
    assert(key == 5);
    assert(val == 50);

    assert(dary_heap_extract_min(heap, &key, &val));
    assert(key == 5);
    assert(val == 50);

    assert(dary_heap_extract_min(heap, &key, &val));
    assert(key == 10);
    assert(val == 100);

    assert(dary_heap_extract_min(heap, &key, &val));
    assert(key == 15);
    assert(val == 150);

    assert(dary_heap_extract_min(heap, &key, &val));
    assert(key == 20);
    assert(val == 200);

    assert(dary_heap_extract_min(heap, &key, &val));
    assert(key == 30);
    assert(val == 300);

    assert(heap->size == 0);
    destroy_dary_heap(heap);
    printf("test_dary_insert_and_extract passed.\n");
}

void test_dary_decrease_key()
{
    printf("Running test_dary_decrease_key...\n");

    // Create a 4-ary heap
    DAryHeap* heap = create_dary_heap(10, 4);
    assert(heap != NULL);

    assert(dary_heap_insert(heap, 20, 200));
    assert(dary_heap_insert(heap, 40, 400));
    assert(dary_heap_insert(heap, 30, 300));
    assert(dary_heap_insert(heap, 50, 500));

    int idx = dary_heap_find_index(heap, 40);
    assert(idx != -1);

    // Decrease key of 40 to 10
    assert(dary_heap_decrease_key(heap, idx, 10));

    int key, val;
    assert(dary_heap_get_min(heap, &key, &val));
    assert(key == 10);
    assert(val == 400); // Verify value stays tied to the decreased node

    destroy_dary_heap(heap);
    printf("test_dary_decrease_key passed.\n");
}

int main()
{
    test_dary_insert_and_extract();
    test_dary_decrease_key();
    printf("All d-Ary Heap tests passed!\n");
    return 0;
}
