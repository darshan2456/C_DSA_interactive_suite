#include "advanced_heaps.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_fib_insert_and_extract()
{
    FibonacciNode* heap = NULL;

    heap = fib_heap_insert(heap, 10, 100);
    heap = fib_heap_insert(heap, 5, 50);
    heap = fib_heap_insert(heap, 15, 150);
    heap = fib_heap_insert(heap, 2, 20);

    int min_key, min_val;

    heap = fib_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 2);
    assert(min_val == 20);

    heap = fib_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 5);
    assert(min_val == 50);

    heap = fib_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 10);
    assert(min_val == 100);

    heap = fib_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 15);
    assert(min_val == 150);

    assert(heap == NULL);

    destroy_fibonacci_heap(heap);
    printf("Fibonacci Heap core insert and extract test passed\n");
}

void test_fib_union()
{
    FibonacciNode* heap1 = NULL;
    heap1 = fib_heap_insert(heap1, 10, 100);
    heap1 = fib_heap_insert(heap1, 30, 300);

    FibonacciNode* heap2 = NULL;
    heap2 = fib_heap_insert(heap2, 5, 50);
    heap2 = fib_heap_insert(heap2, 20, 200);

    FibonacciNode* union_heap = fib_heap_union(heap1, heap2);

    int min_key, min_val;
    union_heap = fib_heap_extract_min(union_heap, &min_key, &min_val);
    assert(min_key == 5);
    assert(min_val == 50);

    union_heap = fib_heap_extract_min(union_heap, &min_key, &min_val);
    assert(min_key == 10);
    assert(min_val == 100);

    destroy_fibonacci_heap(union_heap);
    printf("Fibonacci Heap union test passed\n");
}

void test_fib_decrease_key()
{
    FibonacciNode* heap = NULL;

    heap = fib_heap_insert(heap, 10, 100);
    heap = fib_heap_insert(heap, 20, 200);
    heap = fib_heap_insert(heap, 30, 300);

    FibonacciNode* target = fib_heap_find_node(heap, 20);
    assert(target != NULL);

    heap = fib_heap_decrease_key(heap, target, 5);

    int min_key, min_val;
    heap = fib_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 5);
    assert(min_val == 200);

    destroy_fibonacci_heap(heap);
    printf("Fibonacci Heap decrease key test passed\n");
}

void test_fib_delete()
{
    FibonacciNode* heap = NULL;

    heap = fib_heap_insert(heap, 10, 100);
    heap = fib_heap_insert(heap, 20, 200);
    heap = fib_heap_insert(heap, 30, 300);

    FibonacciNode* target = fib_heap_find_node(heap, 20);
    assert(target != NULL);

    heap = fib_heap_delete(heap, target);

    /* Verify 20 is gone and we extract 10 then 30 */
    int min_key, min_val;
    heap = fib_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 10);
    assert(min_val == 100);

    heap = fib_heap_extract_min(heap, &min_key, &min_val);
    assert(min_key == 30);
    assert(min_val == 300);

    destroy_fibonacci_heap(heap);
    printf("Fibonacci Heap delete test passed\n");
}

int main()
{
    test_fib_insert_and_extract();
    test_fib_union();
    test_fib_decrease_key();
    test_fib_delete();

    printf("All Fibonacci Heap tests passed\n");
    return 0;
}
