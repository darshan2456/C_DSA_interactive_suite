#include "array.h"
#include "dcll.h"
#include "display_header.h"
#include "dll.h"
#include "priority_queue.h"
#include "queue.h"
#include "safe_input.h"
#include "scll.h"
#include "sll.h"
#include "stack.h"
#include <stdio.h>

void data_structures_demo(void)
{
    while (1)
    {
        display_header("Data Structures");

        int data_structures_choice;
        int data_structures_status =
            safe_input_int(&data_structures_choice,
                           "\nenter 1 for standard linear data structures"
                           "\nenter 2 for circular variants of linear data structures"
                           "\nenter choice : ",
                           1, 2);

        if (data_structures_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting data structures demo....\n");
            return;
        }

        if (data_structures_status == 0)
        {
            continue;
        }

        switch (data_structures_choice)
        {

            case 1: // sll,dll and arrays case
                while (1)
                {
                    int linear_ds_choice;
                    int linear_ds_status = safe_input_int(
                        &linear_ds_choice,
                        "\nenter 1 for singly linked list demo"
                        "\nenter 2 for doubly linked list demo"
                        "\nenter 3 for arrays demo"
                        "\nenter 4 for priority queue (binary heap implementation with array) demo"
                        "\nenter 5 for simple (linear) queue demo"
                        "\nenter choice : ",
                        1, 5);

                    if (linear_ds_status == INPUT_EXIT_SIGNAL)
                        break;

                    if (linear_ds_status == 0)
                        continue;

                    if (linear_ds_choice == 1)
                    {
                        display_header("Singly Linked List");
                        sll_demo();
                        continue;
                    }
                    if (linear_ds_choice == 2)
                    {
                        display_header("Doubly Linked List");
                        dll_demo();
                        continue;
                    }
                    if (linear_ds_choice == 3)
                    {
                        display_header("Arrays");
                        array_demo();
                        continue;
                    }
                    if (linear_ds_choice == 4)
                    {
                        display_header("Priority Queue");
                        priority_queue_demo();
                        continue;
                    }
                    if (linear_ds_choice == 5)
                    {
                        display_header("Simple Queue");
                        simple_queue_demo();
                        continue;
                    }
                }
                break;

            case 2: // circular variants case

                while (1)
                {
                    int circular_variant_choice = 0;
                    int circular_variant_status =
                        safe_input_int(&circular_variant_choice,
                                       "\nenter 1 for circular queue demo"
                                       "\nenter 2 for singly circular linked list demo"
                                       "\nenter 3 for doubly circular  linked list demo"
                                       "\nenter 4 for double-ended queue (deque) demo"
                                       "\nenter choice : ",
                                       1, 4);
                    if (circular_variant_status == 0)
                        continue;
                    if (circular_variant_status == INPUT_EXIT_SIGNAL)
                        break;
                    if (circular_variant_choice == 1)
                    {
                        display_header("Circular Queue");
                        circular_queue_demo();
                    }
                    if (circular_variant_choice == 2)
                    {
                        display_header("Singly Circular Linked List");
                        scll_demo();
                    }
                    if (circular_variant_choice == 3)
                    {
                        display_header("Doubly Circular Linked List");
                        dcll_demo();
                    }
                    if (circular_variant_choice == 4)
                    {
                        display_header("Double-Ended Queue (Deque)");
                        deque_demo();
                    }
                }

                break;
        }

        printf("\nreturning to main menu....\n");
    }
}
