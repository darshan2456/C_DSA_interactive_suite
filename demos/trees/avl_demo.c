#include "safe_input.h"
#include "serialization.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

void avl_demo(void)
{
    while (1)
    {
        printf("\n\nAVL Tree Demo");
        int option;
        int option_status = safe_input_int(&option,
                                           "\n1. Build a new AVL tree\n"
                                           "2. Load AVL tree from file\n"
                                           "\nenter option (or '-1' to exit): ",
                                           1, 2);

        if (option_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting AVL tree demo\n");
            return;
        }
        if (option_status == 0)
        {
            continue;
        }

        avlNode* root = NULL;

        if (option == 2)
        {
            char path[256];
            int path_status = safe_input_string(path, "\nenter filepath to load from:- ");
            if (path_status == INPUT_EXIT_SIGNAL)
            {
                continue;
            }
            root = deserialize_avl_from_file(path);
            if (root == NULL)
            {
                printf("\nfailed to load AVL tree from file.\n");
                continue;
            }
            printf("\nAVL tree loaded successfully from '%s'.\n", path);
        }
        else
        {
            int total_nodes;
            int total_nodes_status =
                safe_input_int(&total_nodes,
                               "\n\nenter total number of nodes you want in the AVL tree, "
                               "(between 1 and 100), enter '-1' to exit:- ",
                               1, 100);

            if (total_nodes_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting AVL tree demo\n");
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
        }

        printf("\nheight of the AVL tree is:- %d\n", avl_height(root));

        while (1)
        {
            int traversal_choice;
            int traversal_status =
                safe_input_int(&traversal_choice,
                               "\nenter '1' for inorder, '2' for preorder, "
                               "'3' for postorder, '4' to delete a node, '5' to "
                               "check balance factor, '6' to save to file, and '-1' to exit:- ",
                               1, 6);

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
            else if (traversal_choice == 6)
            {
                char path[256];
                int path_status = safe_input_string(path, "\nenter filepath to save to:- ");
                if (path_status != INPUT_EXIT_SIGNAL)
                {
                    if (serialize_avl_to_file(root, path))
                    {
                        printf("\nAVL tree saved successfully to '%s'.\n", path);
                    }
                    else
                    {
                        printf("\nfailed to save AVL tree to file.\n");
                    }
                }
            }
        }
    }
}
