#include "data_structures.h"
#include "safe_input.h"
#include <stdio.h>

void data_structures_demo(void)
{
    while (1)
    {
        int data_structures_choice;
        int data_structures_status =
            safe_input_int(&data_structures_choice,
                           "\nenter 1 for standard linear data structures"
                           "\nenter 2 for circular variants of linear data structures"
                           "\nenter 3 for non-linear data structures"
                           "\nenter choice : ",
                           1, 3);

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
                        sll_Demo();
                        continue;
                    }
                    if (linear_ds_choice == 2)
                    {
                        dll_demo();
                        continue;
                    }
                    if (linear_ds_choice == 3)
                    {
                        array_demo();
                        continue;
                    }
                    if (linear_ds_choice == 4)
                    {
                        priority_queue_demo();
                        continue;
                    }
                    if (linear_ds_choice == 5)
                    {
                        simple_queue_Demo();
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
                                       "\nenter 3 for double-ended queue (deque) demo"
                                       "\nenter choice : ",
                                       1, 3);
                    if (circular_variant_status == 0)
                        continue;
                    if (circular_variant_status == INPUT_EXIT_SIGNAL)
                        break;
                    if (circular_variant_choice == 1)
                    {
                        circular_queue_Demo();
                    }
                    if (circular_variant_choice == 2)
                    {
                        scll_Demo();
                    }
                    if (circular_variant_choice == 3)
                    {
                        deque_demo();
                    }
                }

                break;

            case 3: // non linear data structures case
                while (1)
                {
                    int non_linear_ds_choice;
                    int non_linear_ds_status =
                        safe_input_int(&non_linear_ds_choice,
                                       "\nenter 1 for binary search tree demo"
                                       "\nenter 2 for threaded binary tree demo"
                                       "\nenter 3 for AVL tree demo"
                                       "\nenter 4 for Trie(prefix tree) demo"
                                       "\nenter choice: ",
                                       1, 4);
                    if (non_linear_ds_status == INPUT_EXIT_SIGNAL)
                        break;
                    if (non_linear_ds_status == 0)
                        continue;
                    if (non_linear_ds_choice == 1)
                    {
                        binary_search_tree_Demo();
                    }
                    if (non_linear_ds_choice == 2)
                    {
                        TBT_demo();
                    }
                    if (non_linear_ds_choice == 3)
                    {
                        avl_demo();
                    }
                    if (non_linear_ds_choice == 4)
                    {
                        trie_demo();
                    }
                }
                break;
        }

        printf("\nreturning to main menu....\n");
    }
}
