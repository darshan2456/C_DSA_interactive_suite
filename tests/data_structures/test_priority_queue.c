#include "data_structures.h"
#include <assert.h>
#include <stdio.h>

void test_init()
{
    priority_queue* pq = pq_init(MIN_HEAP);

    assert(pq != NULL);
    assert(pq->size == 0);
    assert(pq->heapType == MIN_HEAP);

    destroy_pq(pq);

    printf("Priority queue init test passed\n");
}

void test_min_heap()
{
    priority_queue* pq = pq_init(MIN_HEAP);

    insert(pq, 5);
    insert(pq, 3);
    insert(pq, 8);
    insert(pq, 1);

    int value;

    assert(extractTop(pq, &value));
    assert(value == 1);

    assert(extractTop(pq, &value));
    assert(value == 3);

    assert(extractTop(pq, &value));
    assert(value == 5);

    assert(extractTop(pq, &value));
    assert(value == 8);

    destroy_pq(pq);

    printf("Priority queue min heap test passed\n");
}

void test_max_heap()
{
    priority_queue* pq = pq_init(MAX_HEAP);

    insert(pq, 5);
    insert(pq, 3);
    insert(pq, 8);
    insert(pq, 1);

    int value;

    assert(extractTop(pq, &value));
    assert(value == 8);

    assert(extractTop(pq, &value));
    assert(value == 5);

    assert(extractTop(pq, &value));
    assert(value == 3);

    assert(extractTop(pq, &value));
    assert(value == 1);

    destroy_pq(pq);

    printf("Priority queue max heap test passed\n");
}

void test_peek()
{
    priority_queue* pq = pq_init(MIN_HEAP);

    insert(pq, 10);
    insert(pq, 5);
    insert(pq, 20);

    int value;

    assert(peek_pq(pq, &value));
    assert(value == 5);
    assert(pq->size == 3);

    destroy_pq(pq);

    printf("Priority queue peek test passed\n");
}

void test_empty_heap()
{
    priority_queue* pq = pq_init(MIN_HEAP);

    int value;

    assert(!peek_pq(pq, &value));
    assert(!extractTop(pq, &value));

    destroy_pq(pq);

    printf("Priority queue empty heap test passed\n");
}

void test_duplicates()
{
    priority_queue* pq = pq_init(MIN_HEAP);

    insert(pq, 10);
    insert(pq, 10);
    insert(pq, 10);

    int value;

    assert(extractTop(pq, &value));
    assert(value == 10);

    assert(extractTop(pq, &value));
    assert(value == 10);

    assert(extractTop(pq, &value));
    assert(value == 10);

    destroy_pq(pq);

    printf("Priority queue duplicate values test passed\n");
}

void test_overflow()
{
    priority_queue* pq = pq_init(MIN_HEAP);

    for (int i = 0; i < HEAP_CAPACITY; i++)
    {
        assert(insert(pq, i));
    }

    assert(insert(pq, 999) == 0);

    destroy_pq(pq);

    printf("Priority queue overflow test passed\n");
}

void test_alternating_operations()
{
    priority_queue* pq = pq_init(MIN_HEAP);

    int value;

    insert(pq, 10);
    insert(pq, 5);

    assert(extractTop(pq, &value));
    assert(value == 5);

    insert(pq, 3);

    assert(extractTop(pq, &value));
    assert(value == 3);

    insert(pq, 20);

    assert(extractTop(pq, &value));
    assert(value == 10);

    assert(extractTop(pq, &value));
    assert(value == 20);

    destroy_pq(pq);

    printf("Priority queue alternating operations test passed\n");
}

int main()
{
    test_init();
    test_min_heap();
    test_max_heap();
    test_peek();
    test_empty_heap();
    test_duplicates();
    test_overflow();
    test_alternating_operations();

    printf("\nAll priority queue tests passed\n");

    return 0;
}