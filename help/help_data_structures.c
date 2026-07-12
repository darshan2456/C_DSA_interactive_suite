#include "display_header.h"
#include "help.h"
#include "safe_input.h"
#include <stdio.h>

void help_data_structures_menu(void)
{
    while (1)
    {
        display_header("Help - Data Structures");

        printf("Select a sub-topic:\n\n");
        printf("1. Singly Linked List (SLL)\n");
        printf("2. Doubly Linked List (DLL)\n");
        printf("3. Circular Linked Lists (SCLL / DCLL)\n");
        printf("4. Stacks & Queues\n");
        printf("5. Arrays & Priority Queues\n");
        int choice;
        int status = safe_input_int(&choice, "Enter choice (or -1 to go back): ", 1, 5);

        if (status == INPUT_EXIT_SIGNAL)
        {
            break;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                display_header("Help - Singly Linked List (SLL)");
                printf("CONCEPT:\n");
                printf("    A Singly Linked List is a linear data structure where each element "
                       "(node)\n");
                printf(
                    "    contains a data field and a pointer (next) to the next node in line.\n\n");
                printf("OPERATIONS:\n");
                printf("    • Insert (Beginning, Middle, End)\n");
                printf("    • Delete (Beginning, Middle, End)\n");
                printf("    • Search & Traverse\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 2:
                display_header("Help - Doubly Linked List (DLL)");
                printf("CONCEPT:\n");
                printf("    A Doubly Linked List node has three parts: data, next pointer,\n");
                printf("    and a prev pointer referencing the previous node. This allows "
                       "bi-directional traversal.\n\n");
                printf("OPERATIONS:\n");
                printf("    • Insertion & Deletion at arbitrary positions\n");
                printf("    • Reverse Traversal\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 3:
                display_header("Help - Circular Linked Lists");
                printf("CONCEPT:\n");
                printf("    In Circular Linked Lists, the last node links back to the first node "
                       "instead of pointing to NULL.\n");
                printf("    • Singly Circular (SCLL): Single link pointing forward, looping last "
                       "to first.\n");
                printf("    • Doubly Circular (DCLL): Predecessor and successor links form a full "
                       "circular loop.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 4:
                display_header("Help - Stacks & Queues");
                printf("STACK (LIFO - Last In First Out):\n");
                printf("    Supports Push (insert) and Pop (remove) operations. Only the top is "
                       "accessible.\n\n");
                printf("QUEUE (FIFO - First In First Out):\n");
                printf("    Supports Enqueue (insert at rear) and Dequeue (remove from front) "
                       "operations.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 5:
                display_header("Help - Arrays & Priority Queues");
                printf("ARRAYS:\n");
                printf("    Fixed-size sequential collections of elements. Operations include "
                       "search, insertion, and deletion.\n\n");
                printf("PRIORITY QUEUE:\n");
                printf("    A queue where each element has a priority. Implemented here using a "
                       "Binary Heap structure.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;
        }
    }
}
