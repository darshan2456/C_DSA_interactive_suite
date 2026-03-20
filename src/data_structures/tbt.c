#include "tbt.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

TBTnode* leftmost(TBTnode* node)
{
    if (!node)
    {
        return NULL;
    }
    while (node->left && !node->lthread)
    {
        node = node->left;
    }
    return node;
}

TBTnode* create_node_tbt(int data)
{
    TBTnode* node = malloc(sizeof(TBTnode));
    if (!node)
    {
        printf("\nmalloc error while initializing node.");
        return NULL;
    }
    node->data = data;

    node->lthread = node->rthread = true;
    node->left = node->right = NULL;

    return node;
}

void inorder_tbt(TBTnode* node)
{
    TBTnode* curr = leftmost(node);

    while (curr)
    {
        printf("%d->", curr->data);

        if (curr->rthread)
        {
            curr = curr->right;
        }
        else
        {
            curr = leftmost(curr->right);
        }
    }
    printf("end\n");
}

// return 1 on successful insertion, -1 if data is present, -2 on malloc failure and -3 on invalid
// input
int insert_node_tbt(TBTnode** root_ref, int key)
{
    if (!root_ref)
    {
        return -3;
    }

    TBTnode* parent = NULL;
    TBTnode* curr = *root_ref;

    while (curr != NULL)
    {
        if (key == curr->data)
        {
            return -1;
        }

        parent = curr;

        if (key < curr->data)
        {
            if (!curr->lthread)
            {
                curr = curr->left;
            }
            else
                break;
        }
        else
        {
            if (!curr->rthread)
            {
                curr = curr->right;
            }
            else
                break;
        }
    }

    TBTnode* new_node = create_node_tbt(key);

    if (!new_node)
    {
        return -2;
    }

    if (parent == NULL)
    { // case where tree is empty
        *root_ref = new_node;
        return 1;
    }

    if (key < parent->data)
    { // left insertion
        new_node->left = parent->left;
        new_node->right = parent;

        parent->lthread = false;
        parent->left = new_node;
    }
    else // right insertion
    {
        new_node->right = parent->right;
        new_node->left = parent;

        parent->rthread = false;
        parent->right = new_node;
    }

    return 1;
}

void destroy_tbt(TBTnode* node)
{
    TBTnode* curr = leftmost(node);

    while (curr)
    {
        TBTnode* next;
        if (curr->rthread)
        {
            next = curr->right;
        }
        else
        {
            next = leftmost(curr->right);
        }

        free(curr);
        curr = next;
    }
}

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
    retry_capacity:
        int val_status = safe_input_int(
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
            goto retry_capacity;
        }
    }

    for (int i = 0; i < tbt_capacity; i++)
    {

    retry:
        int insert_status = insert_node_tbt(&root, tbt_values[i]);

        switch (insert_status)
        {

            case INPUT_EXIT_SIGNAL:
            {
                printf("\nExiting Threaded Binary Tree demo....\n");
                destroy_tbt(root);
                return;
            }
            case 0:
            {
                goto retry;
            }
            case -1:
            {
                printf("\nsame value already exists in the tree, try again");
                goto retry;
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
    destroy_tbt(root);
}