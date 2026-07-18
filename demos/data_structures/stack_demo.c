#include "display_header.h"
#include "export.h"
#include "safe_input.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            if (push(s, (void*)(intptr_t)val) == 1)
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
                int val = (int)(intptr_t)pop(s);
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
                int val = (int)(intptr_t)peek(s);
                printf("\nTop element: %d\n", val);
            }
            printStackAsInts(s);
        }
    }

    // Export option
    int export_choice;
    int export_status = safe_input_int(
        &export_choice,
        "\nDo you want to export this Stack to a file? (1 for Yes, 2 for No, or '-1' to exit): ", 1,
        2);
    if (export_status != INPUT_EXIT_SIGNAL && export_status != 0 && export_choice == 1)
    {
        char path[256];
        int path_status = safe_input_string(path, "Enter filename to export (e.g. stack.json): ");
        if (path_status != INPUT_EXIT_SIGNAL && strlen(path) > 0)
        {
            int format_val;
            int format_status = safe_input_int(
                &format_val,
                "Select format (1 for Text, 2 for CSV, 3 for JSON, or '-1' to exit): ", 1, 3);
            if (format_status != INPUT_EXIT_SIGNAL && format_status != 0)
            {
                ExportFormat format = EXPORT_FORMAT_TEXT;
                if (format_val == 2)
                    format = EXPORT_FORMAT_CSV;
                else if (format_val == 3)
                    format = EXPORT_FORMAT_JSON;

                if (stack_export(s, path, format, write_data_int))
                {
                    printf("Exported successfully to %s\n", path);
                }
                else
                {
                    printf("Failed to export to %s\n", path);
                }
            }
        }
    }

    destroyStack(s, NULL);
    printf("\nStack destroyed. Returning to Data Structures menu...\n");
}
