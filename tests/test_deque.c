#include "data_structures.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "returnMallocVal.h"

void test_deque_init()
{
    Queue dq;
    assert(init_deque(5, &dq) == 1);
    assert(dq.N == 5);
    assert(dq.front == -1);
    assert(dq.rear == -1);
    assert(deque_is_empty(&dq) == true);
    assert(deque_is_full(&dq) == false);

    destroy_deque(&dq);

    printf("Deque init test passed\n");
}

void test_deque_basic_ops()
{
    Queue dq;
    init_deque(5, &dq);

    assert(deque_insert_rear(&dq, returnMallocInt(10)) == 1);
    assert(deque_insert_rear(&dq, returnMallocInt(20)) == 1);
    assert(deque_insert_front(&dq, returnMallocInt(5)) == 1);

    assert(deque_get_front(&dq) == 5);
    assert(deque_get_rear(&dq) == 20);

    int* val;

    val = deque_delete_front(&dq);
    assert(val != NULL);
    assert(*val == 5);
    free(val);

    val = deque_delete_rear(&dq);
    assert(val != NULL);
    assert(*val == 20);
    free(val);

    val = deque_delete_front(&dq);
    assert(val != NULL);
    assert(*val == 10);
    free(val);

    assert(deque_is_empty(&dq) == true);

    destroy_deque(&dq);

    printf("Deque basic ops test passed\n");
}

void test_deque_underflow()
{
    Queue dq;
    init_deque(5, &dq);

    assert(deque_delete_front(&dq) == NULL);
    assert(deque_delete_rear(&dq) == NULL);
    assert(deque_get_front(&dq) == -1);
    assert(deque_get_rear(&dq) == -1);

    destroy_deque(&dq);

    printf("Deque underflow test passed\n");
}

void test_deque_overflow()
{
    Queue dq;
    init_deque(3, &dq);

    assert(deque_insert_rear(&dq, returnMallocInt(1)) == 1);
    assert(deque_insert_front(&dq, returnMallocInt(2)) == 1);
    assert(deque_insert_rear(&dq, returnMallocInt(3)) == 1);
    assert(deque_is_full(&dq) == true);

    int* ptr1 = returnMallocInt(4);
    assert(ptr1 != NULL);
    assert(deque_insert_front(&dq, ptr1) == -1);
    free(ptr1);

    int* ptr2 = returnMallocInt(5);
    assert(ptr2 != NULL);
    assert(deque_insert_rear(&dq, ptr2) == -1);
    free(ptr2);

    destroy_deque(&dq);

    printf("Deque overflow test passed\n");
}

void test_deque_wraparound()
{
    Queue dq;
    init_deque(3, &dq);

    int* val;

    // Front: -1, Rear: -1
    assert(deque_insert_rear(&dq, returnMallocInt(1)) == 1); // F: 0, R: 0 [1]
    assert(deque_insert_rear(&dq, returnMallocInt(2)) == 1); // F: 0, R: 1 [1, 2]

    val = deque_delete_front(&dq); // F: 1, R: 1 [2]
    assert(val != NULL);
    assert(*val == 1);
    free(val);

    assert(deque_insert_front(&dq, returnMallocInt(3)) == 1); // F: 0, R: 1 [3, 2]
    assert(deque_insert_front(&dq, returnMallocInt(4)) == 1); // F: 2, R: 1 [4, 3, 2]
    assert(deque_is_full(&dq) == true);

    assert(deque_get_front(&dq) == 4);
    assert(deque_get_rear(&dq) == 2);

    val = deque_delete_rear(&dq); // F: 2, R: 0
    assert(val != NULL);
    assert(*val == 2);
    free(val);

    val = deque_delete_front(&dq); // F: 0, R: 0
    assert(val != NULL);
    assert(*val == 4);
    free(val);

    val = deque_delete_front(&dq); // F: -1, R: -1
    assert(val != NULL);
    assert(*val == 3);
    free(val);

    assert(deque_is_empty(&dq) == true);

    destroy_deque(&dq);

    printf("Deque wraparound test passed\n");
}

int main()
{
    test_deque_init();
    test_deque_basic_ops();
    test_deque_underflow();
    test_deque_overflow();
    test_deque_wraparound();

    printf("All Deque tests passed\n");

    return 0;
}