#include <assert.h>
#include <stdio.h>
#include "stack.h"

void test_push_pop_lifo(void)
{
    stack* s = createStack();
    push(s, 10);
    push(s, 20);
    push(s, 30);
    assert(pop(s) == 30);
    assert(pop(s) == 20);
    assert(pop(s) == 10);
    assert(pop(s) == -1);
    destroyStack(s);
    printf("test_push_pop_lifo passed.\n");
}

void test_pop_empty(void)
{
    stack* s = createStack();
    assert(pop(s) == -1);
    push(s, 7);
    assert(pop(s) == 7);
    assert(pop(s) == -1);
    destroyStack(s);
    printf("test_pop_empty passed.\n");
}

void test_peek_no_remove(void)
{
    stack* s = createStack();
    assert(peek(s) == -1);
    push(s, 42);
    assert(peek(s) == 42);
    assert(peek(s) == 42);
    assert(isEmpty(s) == 0);
    pop(s);
    assert(peek(s) == -1);
    destroyStack(s);
    printf("test_peek_no_remove passed.\n");
}

void test_isEmpty(void)
{
    stack* s = createStack();
    assert(isEmpty(s) == 1);
    push(s, 5);
    assert(isEmpty(s) == 0);
    push(s, 10);
    assert(isEmpty(s) == 0);
    pop(s);
    assert(isEmpty(s) == 0);
    pop(s);
    assert(isEmpty(s) == 1);
    destroyStack(s);
    printf("test_isEmpty passed.\n");
}

void test_destroy_nonEmpty(void)
{
    stack* s = createStack();
    push(s, 1);
    push(s, 2);
    push(s, 3);
    destroyStack(s);
    printf("test_destroy_nonEmpty passed.\n");
}

int main(void)
{
    test_push_pop_lifo();
    test_pop_empty();
    test_peek_no_remove();
    test_isEmpty();
    test_destroy_nonEmpty();
    printf("All stack tests passed.\n");
    return 0;
}