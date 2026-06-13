#include "data_structures.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "returnMallocVal.h"

void test_init()
{
    Queue q;
    assert(init_simple_queue(5, &q) == 1);
    assert(q.N == 5);
    assert(q.front == -1);
    assert(q.rear == -1);

    destroy_simple_queue(&q);

    printf("Simple queue init test passed\n");
}

void test_basic_enqueue_dequeue()
{
    Queue q;
    init_simple_queue(5, &q);

    assert(enqueue_simple(&q, returnMallocInt(30)) == 1);
    assert(enqueue_simple(&q, returnMallocInt(20)) == 1);

    int* val;

    val = dequeue_simple(&q);
    assert(val != NULL);
    assert(*val == 30);
    free(val);

    val = dequeue_simple(&q);
    assert(val != NULL);
    assert(*val == 20);
    free(val);

    destroy_simple_queue(&q);

    printf("Simple queue basic enqueue/dequeue test passed\n");
}

void test_underflow()
{
    Queue q;
    init_simple_queue(5, &q);

    assert(dequeue_simple(&q) == NULL);

    destroy_simple_queue(&q);

    printf("Simple queue underflow test passed\n");
}

void test_overflow()
{
    Queue q;
    init_simple_queue(3, &q);

    assert(enqueue_simple(&q, returnMallocInt(1)) == 1);
    assert(enqueue_simple(&q, returnMallocInt(2)) == 1);
    assert(enqueue_simple(&q, returnMallocInt(3)) == 1);

    int* ptr = returnMallocInt(4);
    assert(ptr != NULL);

    assert(enqueue_simple(&q, ptr) == -1);
    free(ptr);

    destroy_simple_queue(&q);

    printf("Simple queue overflow test passed\n");
}

void test_false_overflow()
{
    Queue q;
    init_simple_queue(3, &q);

    enqueue_simple(&q, returnMallocInt(1));
    enqueue_simple(&q, returnMallocInt(2));
    enqueue_simple(&q, returnMallocInt(3));

    int* val;

    val = dequeue_simple(&q);
    assert(val != NULL);
    assert(*val == 1);
    free(val);

    val = dequeue_simple(&q);
    assert(val != NULL);
    assert(*val == 2);
    free(val);

    int* ptr = returnMallocInt(4);
    assert(ptr != NULL);

    assert(enqueue_simple(&q, ptr) == -1);
    free(ptr);

    val = dequeue_simple(&q);
    assert(val != NULL);
    assert(*val == 3);
    free(val);

    assert(dequeue_simple(&q) == NULL);

    destroy_simple_queue(&q);

    printf("Simple queue false-overflow (linear limitation) test passed\n");
}

void test_fifo_order()
{
    Queue q;
    init_simple_queue(4, &q);

    for (int i = 1; i <= 4; i++)
        assert(enqueue_simple(&q, returnMallocInt(i * 10)) == 1);

    for (int i = 1; i <= 4; i++)
    {
        int* val = dequeue_simple(&q);
        assert(val != NULL);
        assert(*val == i * 10);
        free(val);
    }

    destroy_simple_queue(&q);

    printf("Simple queue FIFO order test passed\n");
}

int main()
{
    test_init();
    test_basic_enqueue_dequeue();
    test_underflow();
    test_overflow();
    test_false_overflow();
    test_fifo_order();

    printf("All simple queue tests passed\n");
    return 0;
}