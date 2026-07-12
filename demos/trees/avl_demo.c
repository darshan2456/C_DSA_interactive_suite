#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

void avl_demo(void)
{
    while (1)
    {
        avlNode* root = NULL;
        int total_nodes;
        int total_nodes_status =
            safe_input_int(&total_nodes,
                           "\n\nenter total number of nodes you want in the AVL tree, "
                           "(between 1 and 100), enter '-1' to exit:- ",
                           1, 100);

        if (total_nodes_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting AVL tree demo\n");
            destroy_avl(root);
            return;
        }
        if (total_nodes_status == 0)
        {
            continue;
        }

        int i = 1;
        while (total_nodes > 0)
        {
            int node_value;
            printf("\nenter value of %d AVL node - ", i);
            int node_value_status = safe_input_int(&node_value, NULL, 1, 100);

            if (node_value_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting AVL tree demo\n");
                destroy_avl(root);
                return;
            }
            if (node_value_status == 0)
            {
                continue;
            }

            int insertion_status = avl_insert(&root, node_value);
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
            total_nodes--;
        }

        printf("\nheight of the AVL tree is:- %d\n", avl_height(root));

        while (1)
        {
            int traversal_choice;
            int traversal_status = safe_input_int(&traversal_choice,
                                                  "\nenter '1' for inorder, '2' for preorder and "
                                                  "'3' for postorder, '4' to delete a node, '5' to "
                                                  "check balance factor, and '-1' to exit:- ",
                                                  1, 5);

            if (traversal_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting AVL tree demo\n");
                destroy_avl(root);
                return;
            }
            if (traversal_status == 0)
            {
                continue;
            }

            if (traversal_choice == 1)
            {
                avl_inorder(root);
                printf("\n");
            }
            else if (traversal_choice == 2)
            {
                avl_preorder(root);
                printf("\n");
            }
            else if (traversal_choice == 3)
            {
                avl_postorder(root);
                printf("\n");
            }
            else if (traversal_choice == 4)
            {
                int delete_value;
                int delete_status;
                bool cancelled = false;
                while (1)
                {
                    delete_status = safe_input_int(
                        &delete_value,
                        "\nenter value to delete (between 1 and 100), enter '-1' to exit:- ", 1,
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
                int status = avl_delete(&root, delete_value);
                if (status == 0)
                {
                    printf("\nvalue not found in the tree\n");
                }
                else
                {
                    printf("\nnode deleted. updated inorder traversal: ");
                    avl_inorder(root);
                    printf("\n");
                }
            }
            else if (traversal_choice == 5)
            {
                printf("\nbalance factor of root node is: %d\n", avl_balance_factor(root));
            }
        }
    }
}
