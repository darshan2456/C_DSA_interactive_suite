#include "data_structures.h"
#include <stdio.h>
#include <stdlib.h>

int push(stack* s, int value)
{
    if (sll_insertAtBeginning(&(s->top), value))
    {
        return 1;
    }
    return 0;
}

int pop(stack* s)
{
    if (s->top == NULL)
        return -1;
    int top_of_stack = s->top->data;
    sll_deleteAtBeginning(&(s->top));
    return top_of_stack;
}

stack* createStack(void)
{
    stack* stack_obj = malloc(sizeof(stack));
    if (stack_obj == NULL)
        return NULL;
    stack_obj->top = NULL;
    return stack_obj;
}

int isEmpty(const stack* s)
{
    return s->top == NULL;
}

void destroyStack(stack* s)
{
    if (s == NULL)
        return;
    while (!isEmpty(s))
    {
        pop(s);
    }
    free(s);
}

int peek(const stack* s)
{
    if (s->top == NULL)
        return -1;
    return s->top->data;
}

/**
 * Prints stack contents as characters from top to bottom for infix-to-postfix visualization.
 * The format is: Stack (top -> bottom): | + | * |
 */
void printStack(const stack* s)
{
    if (s == NULL || s->top == NULL)
    {
        printf("Stack is empty.\n");
        return;
    }
    printf("Stack (top -> bottom): ");
    Node* curr = s->top;
    while (curr != NULL)
    {
        printf("| %c ", curr->data);
        curr = curr->next;
    }
    printf("|\n");
}

/**
 * Prints stack contents as integers from top to bottom for postfix evaluation visualization.
 * The format is: Stack (top -> bottom): | 99 | 11 |
 */
void printStackAsInts(const stack* s)
{
    if (s == NULL || s->top == NULL)
    {
        printf("Stack is empty.\n");
        return;
    }
    printf("Stack (top -> bottom): ");
    Node* curr = s->top;
    while (curr != NULL)
    {
        printf("| %d ", curr->data);
        curr = curr->next;
    }
    printf("|\n");
}
