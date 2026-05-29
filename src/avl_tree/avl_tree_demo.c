#include "avl_tree.h"
#include "safe_input.h"
#include <stdio.h>

// prints each node with its height and balance factor so the user can see
// why a rotation happened
static void print_node_details(const avlNode* node)
{
    if (node == NULL)
        return;
    printf("  node=%d (h=%d, bf=%d)\n", node->data, node->height, avl_balance_factor(node));
    print_node_details(node->left);
    print_node_details(node->right);
}

static void print_tree_state(const avlNode* root)
{
    if (root == NULL)
    {
        printf("  (empty tree)\n");
        return;
    }
    printf("  inorder:  ");
    avl_inorder(root);
    printf("\n  preorder: ");
    avl_preorder(root);
    printf("\n  height: %d\n", avl_height(root));
    print_node_details(root);
}

static int avl_insertion_phase(avlNode** root_ref)
{
    int total_nodes;
    int status = safe_input_int(&total_nodes,
                                "\nenter total number of nodes to insert in the AVL tree, "
                                "(between 1 and 100), enter '-1' to exit:- ",
                                1, 100);

    if (status == INPUT_EXIT_SIGNAL)
        return INPUT_EXIT_SIGNAL;
    if (status == 0)
        return 0;

    int i = 1;
    while (total_nodes > 0)
    {
        int value;
        printf("\nenter value of node %d (between 1 and 100), enter '-1' to exit:- ", i);
        int val_status = safe_input_int(&value, NULL, 1, 100);

        if (val_status == INPUT_EXIT_SIGNAL)
            return INPUT_EXIT_SIGNAL;
        if (val_status == 0)
            continue;

        int ins_status = avl_insert(root_ref, value);
        if (ins_status == 0)
        {
            printf("\nvalue already exists in the tree, only unique values please\n");
            continue;
        }
        if (ins_status == -1)
        {
            printf("\ncouldnt insert node due to malloc failure. try again\n");
            continue;
        }

        printf("\ntree after inserting %d:\n", value);
        print_tree_state(*root_ref);

        i++;
        total_nodes--;
    }
    return 1;
}

static int avl_operations_menu(avlNode** root_ref)
{
    while (1)
    {
        int choice;
        int status = safe_input_int(&choice,
                                    "\nenter '1' to delete a node"
                                    "\nenter '2' to search a value"
                                    "\nenter '3' for inorder traversal"
                                    "\nenter '4' for preorder traversal"
                                    "\nenter '5' for postorder traversal"
                                    "\nenter '6' to show the tree state"
                                    "\nenter '-1' to exit:- ",
                                    1, 6);

        if (status == INPUT_EXIT_SIGNAL)
            return INPUT_EXIT_SIGNAL;
        if (status == 0)
            continue;

        if (choice == 1)
        {
            int value;
            int v_status = safe_input_int(
                &value, "\nenter value to delete (between 1 and 100), enter '-1' to exit:- ", 1,
                100);

            if (v_status == INPUT_EXIT_SIGNAL)
                return INPUT_EXIT_SIGNAL;
            if (v_status == 0)
                continue;

            int del_status = avl_delete(root_ref, value);
            if (del_status == 0)
            {
                printf("\nvalue not found in the tree\n");
                continue;
            }
            printf("\ntree after deleting %d:\n", value);
            print_tree_state(*root_ref);
        }
        else if (choice == 2)
        {
            int value;
            int v_status = safe_input_int(
                &value, "\nenter value to search (between 1 and 100), enter '-1' to exit:- ", 1,
                100);

            if (v_status == INPUT_EXIT_SIGNAL)
                return INPUT_EXIT_SIGNAL;
            if (v_status == 0)
                continue;

            if (avl_search(*root_ref, value))
                printf("\nvalue %d is present in the tree\n", value);
            else
                printf("\nvalue %d is not present in the tree\n", value);
        }
        else if (choice == 3)
        {
            printf("\ninorder: ");
            avl_inorder(*root_ref);
            printf("\n");
        }
        else if (choice == 4)
        {
            printf("\npreorder: ");
            avl_preorder(*root_ref);
            printf("\n");
        }
        else if (choice == 5)
        {
            printf("\npostorder: ");
            avl_postorder(*root_ref);
            printf("\n");
        }
        else if (choice == 6)
        {
            printf("\ncurrent tree state:\n");
            print_tree_state(*root_ref);
        }
    }
}

void avl_tree_demo(void)
{
    while (1)
    {
        avlNode* root = NULL;

        int phase_status = avl_insertion_phase(&root);

        if (phase_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting AVL tree demo\n");
            destroy_avl(root);
            return;
        }
        if (phase_status == 0)
        {
            destroy_avl(root);
            continue;
        }

        int op_status = avl_operations_menu(&root);

        destroy_avl(root);

        if (op_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting AVL tree demo\n");
            return;
        }
    }
}