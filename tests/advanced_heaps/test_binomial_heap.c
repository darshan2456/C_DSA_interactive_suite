#include "advanced_heaps.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_binomial_insert_and_extract()
{
    BinomialNode* heap = NULL;

    heap = binomial_heap_insert(heap, 10, 100);
    heap = binomial_heap_insert(heap, 5, 50);
    heap = binomial_heap_insert(heap, 15, 150);
    heap = binomial_heap_insert(heap, 2, 20);

    int min_key, min_val;

    heap = binomial_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 2);
    assert(min_val == 20);

    heap = binomial_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 5);
    assert(min_val == 50);

    heap = binomial_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 10);
    assert(min_val == 100);

    heap = binomial_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 15);
    assert(min_val == 150);

    assert(heap == NULL);

    destroy_binomial_heap(heap);
    printf("Binomial Heap insert and extract test passed\n");
}

void test_binomial_decrease_key()
{
    BinomialNode* heap = NULL;

    heap = binomial_heap_insert(heap, 10, 100);
    heap = binomial_heap_insert(heap, 20, 200);
    heap = binomial_heap_insert(heap, 30, 300);

    BinomialNode* target = binomial_heap_find_node(heap, 20);
    assert(target != NULL);

    heap = binomial_heap_decrease_key(heap, target, 5);

    int min_key, min_val;
    heap = binomial_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 5);
    assert(min_val == 200); /* Value stays tied to decreased key */

    destroy_binomial_heap(heap);
    printf("Binomial Heap decrease key test passed\n");
}

void test_binomial_merge()
{
    BinomialNode* heap1 = NULL;
    heap1 = binomial_heap_insert(heap1, 10, 100);
    heap1 = binomial_heap_insert(heap1, 30, 300);

    BinomialNode* heap2 = NULL;
    heap2 = binomial_heap_insert(heap2, 5, 50);
    heap2 = binomial_heap_insert(heap2, 20, 200);

    BinomialNode* merged = binomial_heap_merge(heap1, heap2);

    int min_key, min_val;
    merged = binomial_heap_extract_min(merged, &min_key, &min_val);
    assert(min_key == 5);

    merged = binomial_heap_extract_min(merged, &min_key, &min_val);
    assert(min_key == 10);

    destroy_binomial_heap(merged);
    printf("Binomial Heap merge test passed\n");
}

int main()
{
    test_binomial_insert_and_extract();
    test_binomial_decrease_key();
    test_binomial_merge();

    printf("All Binomial Heap tests passed\n");
    return 0;
}
