#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/expression_evaluation/expression.h"
#include "../src/expression_evaluation/stack.h"

void test_isOperator()
{
    printf("Running test_isOperator...\n");

    assert(isOperator('+') == 1);
    assert(isOperator('-') == 1);
    assert(isOperator('*') == 1);
    assert(isOperator('/') == 1);
    assert(isOperator('a') == 0);
    assert(isOperator('5') == 0);
    assert(isOperator('(') == 0);
    assert(isOperator(')') == 0);
    assert(isOperator('^') == 0);

    printf("--> test_isOperator PASSED!\n");
}

void test_precedence()
{
    printf("Running test_precedence...\n");

    assert(precedence('*') == 2);
    assert(precedence('/') == 2);

    assert(precedence('+') == 1);
    assert(precedence('-') == 1);

    assert(precedence('(') == -1);
    assert(precedence(')') == -1);
    assert(precedence('A') == -1);

    printf("--> test_precedence PASSED!\n");
}

void test_stack_operations()
{
    printf("Running test_stack_operations...\n");

    stack* s = createStack();
    assert(s != NULL);
    assert(isEmpty(s) == 1);

    assert(push(s, '+') == 1);
    assert(isEmpty(s) == 0);

    assert(push(s, '*') == 1);

    assert(peek(s) == '*');

    assert(pop(s) == '*');
    assert(pop(s) == '+');

    assert(isEmpty(s) == 1);

    assert(pop(s) == -1);
    assert(peek(s) == -1);

    destroyStack(s);
    printf("--> test_stack_operations PASSED!\n");
}

int main()
{
    test_isOperator();
    test_precedence();
    test_stack_operations();
    printf("All Expression Helper Tests Passed! \n");
    return 0;
}