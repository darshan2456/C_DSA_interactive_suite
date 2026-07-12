#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

void TBT_demo(void)
{
    TBTnode* root = NULL;
    int tbt_capacity;

    while (1)
    {
        int capacity_status =
            safe_input_int(&tbt_capacity, "\nenter capacity of the threaded binary tree: ", 1, 10);

        if (capacity_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Threaded Binary Tree demo....\n");
            destroy_tbt(root);
            return;
        }
        if (capacity_status == 0)
        {
            continue;
        }
        break;
    }

    int tbt_values[tbt_capacity];

    for (int i = 0; i < tbt_capacity; i++)
    {
        int val_status;
    retry_input:
        val_status = safe_input_int(
            &tbt_values[i],
            "\nEnter the value to insert: (between 1 and 100), enter '-1' to exit: ", 1, 100);

        if (val_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Threaded Binary Tree demo....\n");
            destroy_tbt(root);
            return;
        }
        if (val_status == 0)
        {
            goto retry_input;
        }

        // Check if value already exists in the previously entered elements
        for (int j = 0; j < i; j++)
        {
            if (tbt_values[j] == tbt_values[i])
            {
                printf("\nsame value already exists, enter a unique value.\n");
                goto retry_input;
            }
        }
    }

    for (int i = 0; i < tbt_capacity; i++)
    {
        int insert_status;
    retry:
        insert_status = insert_node_tbt(&root, tbt_values[i]);

        switch (insert_status)
        {
            case -1:
            {
                printf("\nsame value already exists in the tree, skipping");
                break;
            }
            case -2:
            {
                printf("\nmalloc failure. try again");
                goto retry;
            }
            case -3:
            {
                printf("\ninvalid input. try again");
                goto retry;
            }
        }
    }

    printf("\ninorder traversal is: ");
    inorder_tbt(root);
    printf("\npreorder traversal is: ");
    preorder_tbt(root);
    destroy_tbt(root);
}