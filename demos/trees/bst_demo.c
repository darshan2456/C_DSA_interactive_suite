#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>
extern int deletionStrategy;
#define SUCCESSOR 1
#define PREDECESSOR 2

void binary_search_tree_demo(void)
{

    while (1)
    {
        bstNode* head = NULL;
        int total_bst_nodes;
        int total_bst_nodes_status;

        printf("\n");
        total_bst_nodes_status = safe_input_int(&total_bst_nodes,
                                                "enter total number of nodes you want in the bst, "
                                                "(between 1 and 100), enter '-1' to exit:- ",
                                                1, 100);

        if (total_bst_nodes_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting binary search tree demo\n");
            destroy_bst(head);
            return;
        }
        if (total_bst_nodes_status == 0)
        {
            continue;
        }

        int i = 1;
        while (total_bst_nodes > 0)
        {
            int bst_node_value;
            int bst_node_value_status;
            printf("\nenter value of %d bst node - ", i);
            bst_node_value_status = safe_input_int(&bst_node_value, NULL, 1, 100);

            if (bst_node_value_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting binary search tree demo\n");
                destroy_bst(head);
                return;
            }
            if (bst_node_value_status == 0)
            {
                continue;
            }

            int insertion_status = bst_insert(&head, bst_node_value);
            if (insertion_status == 0)
            {
                printf("\nentered same value. only unique values please");
                continue;
            }
            if (insertion_status == -1)
            {
                printf("\ncouldnt insert node due to malloc failure. try again\n");
                continue;
            }
            i++;
            total_bst_nodes--;
        }

        printf("\nheight of the tree is:- %d\n", tree_height(head));

        while (1)
        {
            int bst_traversal_choice;
            int bst_traversal_status;

            bst_traversal_status =
                safe_input_int(&bst_traversal_choice,
                               "\nenter '1' for inorder, '2' for preorder and "
                               "'3' for postorder, '4' for level order, '5' to delete a node, '6' "
                               "to change deletion strategy  and '-1' to exit: ",
                               1, 6);

            if (bst_traversal_status == INPUT_EXIT_SIGNAL)
            {
                destroy_bst(head);
                break;
            }
            if (bst_traversal_status == 0)
            {
                continue;
            }
            if (bst_traversal_choice == 1)
            {
                bst_inorder(head);
            }
            else if (bst_traversal_choice == 2)
            {
                bst_preorder(head);
            }
            else if (bst_traversal_choice == 3)
            {
                bst_postorder(head);
            }
            else if (bst_traversal_choice == 4)
            {
                bst_level_order(head);
            }
            else if (bst_traversal_choice == 5)
            {
                int delete_value;
                int delete_status;
                bool cancelled = false;
                while (1)
                {
                    delete_status = safe_input_int(
                        &delete_value,
                        "\nenter value to delete, (between 1 and 100), enter '-1' to exit:- ", 1,
                        100);
                    if (delete_status == INPUT_EXIT_SIGNAL)
                    {
                        cancelled = true;
                        break;
                    }
                    if (delete_status == 0)
                        continue;
                    break;
                }
                if (cancelled)
                    continue;
                head = bst_delete(head, delete_value);
                printf("\nnode deleted. updated inorder traversal: ");
                bst_inorder(head);
                printf("\n");
            }
            else if (bst_traversal_choice == 6)
            {

                int strategy;
                int strategy_status;
                strategy_status = safe_input_int(&strategy,
                                                 "\nChoose deletion strategy:\n"
                                                 "1. Inorder Successor\n"
                                                 "2. Inorder Predecessor\n"
                                                 "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                                 1, 2);

                if (strategy_status == INPUT_EXIT_SIGNAL)
                    continue;

                if (strategy_status == 0)
                    continue;

                deletionStrategy = strategy;
                if (deletionStrategy == SUCCESSOR)
                    printf("\nDeletion strategy set to Inorder Successor.\n");
                else
                    printf("\nDeletion strategy set to Inorder Predecessor.\n");
            }
        }
    }
}