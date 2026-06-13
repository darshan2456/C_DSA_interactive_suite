#include "data_structures.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "returnMallocVal.h"

void test_init()
{
    Queue q;
    assert(init_circ_queue(5, &q) == 1);
    assert(q.N == 5);
    assert(q.front == 0);
    assert(q.rear == 0);

    destroy_circ_queue(&q);

    printf("Circular queue init test passed\n");
}

void test_basic_enqueue_dequeue()
{
    Queue q;
    init_circ_queue(5, &q);

    assert(enqueue(&q, returnMallocInt(30)) == 1);
    assert(enqueue(&q, returnMallocInt(20)) == 1);

    int* val;

    val = dequeue(&q);
    assert(val != NULL);
    assert(*val == 30);
    free(val);

    val = dequeue(&q);
    assert(val != NULL);
    assert(*val == 20);
    free(val);

    destroy_circ_queue(&q);

    printf("Circular queue basic enqueue/dequeue test passed\n");
}

void test_underflow()
{
    Queue q;
    init_circ_queue(5, &q);

    assert(dequeue(&q) == NULL);

    destroy_circ_queue(&q);

    printf("Circular queue underflow test passed\n");
}

void test_overflow()
{
    Queue q;
    init_circ_queue(4, &q);

    assert(enqueue(&q, returnMallocInt(1)) == 1);
    assert(enqueue(&q, returnMallocInt(2)) == 1);
    assert(enqueue(&q, returnMallocInt(3)) == 1);

    int* ptr = returnMallocInt(4);
    assert(ptr != NULL);

    assert(enqueue(&q, ptr) == -1);
    free(ptr);

    destroy_circ_queue(&q);

    printf("Circular queue overflow test passed\n");
}

void test_wraparound()
{
    Queue q;
    init_circ_queue(4, &q);

    enqueue(&q, returnMallocInt(1));
    enqueue(&q, returnMallocInt(2));
    enqueue(&q, returnMallocInt(3));

    int* val;

    val = dequeue(&q);
    assert(val != NULL);
    assert(*val == 1);
    free(val);

    val = dequeue(&q);
    assert(val != NULL);
    assert(*val == 2);
    free(val);

    assert(enqueue(&q, returnMallocInt(4)) == 1);
    assert(enqueue(&q, returnMallocInt(5)) == 1);

    val = dequeue(&q);
    assert(val != NULL);
    assert(*val == 3);
    free(val);

    val = dequeue(&q);
    assert(val != NULL);
    assert(*val == 4);
    free(val);

    val = dequeue(&q);
    assert(val != NULL);
    assert(*val == 5);
    free(val);

    destroy_circ_queue(&q);

    printf("Circular queue wraparound test passed\n");
}

int main()
{
    test_init();
    test_basic_enqueue_dequeue();
    test_underflow();
    test_overflow();
    test_wraparound();

    printf("All circular queue tests passed\n");
    return 0;
}