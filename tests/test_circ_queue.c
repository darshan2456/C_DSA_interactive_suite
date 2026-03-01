#include "assert.h"
#include "circular_queue.h"
#include <stdio.h>

void test_init() {
    circular_queue q;
    assert(init_circ_queue(5, &q) == 1);
    assert(q.N == 5);
    assert(q.front == 0);
    assert(q.rear == 0);
    destroy_circ_queue(&q);
}

void test_basic_enqueue_dequeue() {
    circular_queue q;
    init_circ_queue(5, &q);

    assert(enqueue(&q, 10) == 1);
    assert(enqueue(&q, 20) == 1);

    assert(dequeue(&q) == 10);
    assert(dequeue(&q) == 20);

    destroy_circ_queue(&q);
}

void test_underflow() {
    circular_queue q;
    init_circ_queue(5, &q);

    assert(dequeue(&q) == -1);

    destroy_circ_queue(&q);
}

void test_overflow() {
    circular_queue q;
    init_circ_queue(4, &q);

    assert(enqueue(&q, 1) == 1);
    assert(enqueue(&q, 2) == 1);
    assert(enqueue(&q, 3) == 1);

    assert(enqueue(&q, 4) == -1);  // overfloew point

    destroy_circ_queue(&q);
}

void test_wraparound() {
    circular_queue q;
    init_circ_queue(4, &q);

    enqueue(&q, 1);
    enqueue(&q, 2);
    enqueue(&q, 3);

    assert(dequeue(&q) == 1);
    assert(dequeue(&q) == 2);

    assert(enqueue(&q, 4) == 1);
    assert(enqueue(&q, 5) == 1);

    assert(dequeue(&q) == 3);
    assert(dequeue(&q) == 4);
    assert(dequeue(&q) == 5);

    destroy_circ_queue(&q);
}

int main() {
    test_init();
    test_basic_enqueue_dequeue();
    test_underflow();
    test_overflow();
    test_wraparound();

    printf("All circular queue tests passed\n");
    return 0;
}