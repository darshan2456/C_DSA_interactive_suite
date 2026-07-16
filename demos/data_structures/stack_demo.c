#include "display_header.h"
#include "safe_input.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

void stack_demo(void)
{
    stack* s = createStack();
    if (s == NULL)
    {
        printf("\nFailed to allocate memory for the Stack.\n");
        return;
    }

    printf("\nStack initialized. Operations are performed LIFO (Last In First Out).\n");
    printStackAsInts(s);

    while (1)
    {
        int choice;
        int status = safe_input_int(&choice,
                                    "\n--- Stack Operations Menu ---\n"
                                    "1. Push (Insert element onto Stack)\n"
                                    "2. Pop (Remove element from Stack)\n"
                                    "3. Peek (View top element)\n"
                                    "enter choice ('-1' to exit, or 'help') : ",
                                    1, 3);

        if (status == INPUT_EXIT_SIGNAL)
        {
            break;
        }

        if (status == 0)
        {
            continue;
        }

        if (choice == 1)
        {
            int val;
            int val_status = safe_input_int(
                &val, "\nEnter integer value to push (1 to 100), or -1 to exit: ", 1, 100);
            if (val_status == INPUT_EXIT_SIGNAL)
            {
                break;
            }
            if (val_status == 0)
            {
                continue;
            }
            if (push(s, val) == 1)
            {
                printf("\nSuccessfully pushed %d onto the stack.\n", val);
            }
            else
            {
                printf("\nFailed to push %d onto the stack (allocation failure).\n", val);
            }
            printStackAsInts(s);
        }
        else if (choice == 2)
        {
            if (isEmpty(s))
            {
                printf("\nStack Underflow: Cannot pop from an empty stack.\n");
            }
            else
            {
                int val = pop(s);
                printf("\nPopped element: %d\n", val);
            }
            printStackAsInts(s);
        }
        else if (choice == 3)
        {
            if (isEmpty(s))
            {
                printf("\nStack is empty: No element to peek.\n");
            }
            else
            {
                int val = peek(s);
                printf("\nTop element: %d\n", val);
            }
            printStackAsInts(s);
        }
    }

    destroyStack(s);
    printf("\nStack destroyed. Returning to Data Structures menu...\n");
}
