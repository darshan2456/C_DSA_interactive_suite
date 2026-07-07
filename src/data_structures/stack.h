#ifndef STACK_H
#define STACK_H

#include "sll.h"
#include <stdbool.h>

// For Stack
typedef struct stack
{
    Node* top;
} stack;

// Create a new stack. Returns Pointer to the created stack, or NULL on failure.
stack* createStack(void);

// Push a value onto the stack. Returns 1 on success, -1 if the stack is invalid.
int push(stack* s, int value);

// Pop a value from the stack. Returns The popped value.
int pop(stack* s);

// Check if the stack is empty. Returns true if empty, false otherwise.
bool isEmpty(const stack* s);

// Destroy a stack and free its resources.
void destroyStack(stack* s);

// Peek at the top value of the stack without popping it. Returns The top value.
int peek(const stack* s);

// Print the stack contents using characters.
void printStack(const stack* s);

// Print the stack contents using integer values.
void printStackAsInts(const stack* s);

#endif
