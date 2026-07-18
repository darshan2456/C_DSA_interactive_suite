#include "stack.h"
#include <assert.h>
#include <stdio.h>

void test_printStack_empty(void)
{
    stack* s = createStack();
    printStack(s);
    assert(isEmpty(s) == 1);
    destroyStack(s, NULL);
    printf("test_printStack_empty passed.\n");
}

void test_printStack_nonempty(void)
{
    stack* s = createStack();
    push(s, (void*)(intptr_t)1);
    push(s, (void*)(intptr_t)2);
    push(s, (void*)(intptr_t)3);
    assert((int)(intptr_t)peek(s) == 3);
    assert(isEmpty(s) == 0);
    assert((int)(intptr_t)pop(s) == 3);
    assert((int)(intptr_t)pop(s) == 2);
    assert((int)(intptr_t)pop(s) == 1);
    assert(isEmpty(s) == 1);
    destroyStack(s, NULL);
    printf("test_printStack_nonempty passed.\n");
}

void test_printStackAsInts_empty(void)
{
    stack* s = createStack();
    printStackAsInts(s);
    assert(isEmpty(s) == 1);
    destroyStack(s, NULL);
    printf("test_printStackAsInts_empty passed.\n");
}

void test_printStackAsInts_nonempty(void)
{
    stack* s = createStack();
    push(s, (void*)(intptr_t)5);
    push(s, (void*)(intptr_t)11);
    push(s, (void*)(intptr_t)99);
    assert((int)(intptr_t)peek(s) == 99);
    assert(isEmpty(s) == 0);
    assert((int)(intptr_t)pop(s) == 99);
    assert((int)(intptr_t)pop(s) == 11);
    assert((int)(intptr_t)pop(s) == 5);
    assert(isEmpty(s) == 1);
    destroyStack(s, NULL);
    printf("test_printStackAsInts_nonempty passed.\n");
}

void test_push_pop_lifo(void)
{
    stack* s = createStack();
    push(s, (void*)(intptr_t)10);
    push(s, (void*)(intptr_t)20);
    push(s, (void*)(intptr_t)30);
    assert((int)(intptr_t)pop(s) == 30);
    assert((int)(intptr_t)pop(s) == 20);
    assert((int)(intptr_t)pop(s) == 10);
    assert(pop(s) == NULL);
    destroyStack(s, NULL);
    printf("test_push_pop_lifo passed.\n");
}

void test_pop_empty(void)
{
    stack* s = createStack();
    assert(pop(s) == NULL);
    push(s, (void*)(intptr_t)7);
    assert((int)(intptr_t)pop(s) == 7);
    assert(pop(s) == NULL);
    destroyStack(s, NULL);
    printf("test_pop_empty passed.\n");
}

void test_peek_no_remove(void)
{
    stack* s = createStack();
    assert(peek(s) == NULL);
    push(s, (void*)(intptr_t)42);
    assert((int)(intptr_t)peek(s) == 42);
    assert((int)(intptr_t)peek(s) == 42);
    assert(isEmpty(s) == 0);
    pop(s);
    assert(peek(s) == NULL);
    destroyStack(s, NULL);
    printf("test_peek_no_remove passed.\n");
}

void test_isEmpty(void)
{
    stack* s = createStack();
    assert(isEmpty(s) == 1);
    push(s, (void*)(intptr_t)5);
    assert(isEmpty(s) == 0);
    push(s, (void*)(intptr_t)10);
    assert(isEmpty(s) == 0);
    pop(s);
    assert(isEmpty(s) == 0);
    pop(s);
    assert(isEmpty(s) == 1);
    destroyStack(s, NULL);
    printf("test_isEmpty passed.\n");
}

void test_destroy_nonEmpty(void)
{
    stack* s = createStack();
    push(s, (void*)(intptr_t)1);
    push(s, (void*)(intptr_t)2);
    push(s, (void*)(intptr_t)3);
    destroyStack(s, NULL);
    printf("test_destroy_nonEmpty passed.\n");
}

int main(void)
{
    test_push_pop_lifo();
    test_pop_empty();
    test_peek_no_remove();
    test_isEmpty();
    test_destroy_nonEmpty();
    test_printStack_empty();
    test_printStack_nonempty();
    test_printStackAsInts_empty();
    test_printStackAsInts_nonempty();
    printf("All stack tests passed.\n");
    return 0;
}
