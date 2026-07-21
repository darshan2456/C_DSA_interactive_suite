#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

static void print_treap_tree(TreapNode* root, int indent)
{
    if (root == NULL)
        return;
    print_treap_tree(root->right, indent + 8);
    for (int j = 0; j < indent; j++)
        printf(" ");
    printf("[%d:%d (prio: %d)]\n", root->key, root->value, root->priority);
    print_treap_tree(root->left, indent + 8);
}

void run_treap_demo(void)
{
    TreapNode* root = NULL;
    while (1)
    {
        display_header("Treap (Tree + Heap) Interactive Demo");
        if (root == NULL)
        {
            printf("\nTreap is empty.\n");
        }
        else
        {
            printf("\nTreap Structure [Key:Value (Priority)]:\n");
            print_treap_tree(root, 0);
        }
        printf("\n1. Insert node\n"
               "2. Delete node\n"
               "3. Get min key node\n"
               "4. Exit treap demo\n");

        int choice;
        if (safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 4) != 1)
            break;
        if (choice == 4)
            break;

        if (choice == 1)
        {
            int key, val, prio;
            if (safe_input_int(&key, "Enter key (BST order): ", -100000, 100000) != 1)
                continue;
            if (safe_input_int(&val, "Enter value: ", -100000, 100000) != 1)
                continue;
            if (safe_input_int(&prio, "Enter priority (Heap order): ", -100000, 100000) != 1)
                continue;
            root = treap_insert(root, key, val, prio);
        }
        else if (choice == 2)
        {
            int key;
            if (safe_input_int(&key, "Enter key to delete: ", -100000, 100000) != 1)
                continue;
            root = treap_delete(root, key);
            printf("\nNode deleted.\n");
            printf("\nPress Enter to continue...");
            getchar();
        }
        else if (choice == 3)
        {
            TreapNode* min_node = treap_get_min(root);
            if (min_node != NULL)
            {
                printf("\nMin Key Node -> Key: %d, Value: %d, Priority: %d\n", min_node->key,
                       min_node->value, min_node->priority);
            }
            else
            {
                printf("\nTreap is empty!\n");
            }
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
    destroy_treap(root);
}
