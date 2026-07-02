#include "advanced_heaps.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_leftist_heap()
{
    LeftistNode* heap = NULL;

    heap = leftist_heap_insert(heap, 15, 150);
    heap = leftist_heap_insert(heap, 10, 100);
    heap = leftist_heap_insert(heap, 30, 300);
    heap = leftist_heap_insert(heap, 5, 50);

    int min_key, min_val;

    heap = leftist_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 5);
    assert(min_val == 50);

    /* Test Leftist property: NPL of left child should be >= right child */
    if (heap != NULL)
    {
        int left_npl = heap->left ? heap->left->npl : -1;
        int right_npl = heap->right ? heap->right->npl : -1;
        assert(left_npl >= right_npl);
    }

    heap = leftist_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 10);

    destroy_leftist_heap(heap);
    printf("Leftist Heap tests passed\n");
}

void test_skew_heap()
{
    SkewNode* heap = NULL;

    heap = skew_heap_insert(heap, 25, 250);
    heap = skew_heap_insert(heap, 5, 50);
    heap = skew_heap_insert(heap, 12, 120);

    int min_key, min_val;

    heap = skew_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 5);
    assert(min_val == 50);

    heap = skew_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 12);
    assert(min_val == 120);

    destroy_skew_heap(heap);
    printf("Skew Heap tests passed\n");
}

int main()
{
    test_leftist_heap();
    test_skew_heap();

    printf("All Leftist and Skew Heap tests passed\n");
    return 0;
}
