#include "safe_input.h"
#include "trees.h"
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

void preorder_tbt(TBTnode* node)
{
    if (node == NULL)
        return;

    TBTnode* curr = node;

    while (curr != NULL)
    {
        printf("%d->", curr->data);

        if (!curr->lthread)
        {
            curr = curr->left;
        }
        else if (!curr->rthread)
        {
            curr = curr->right;
        }
        else
        {
            while (curr != NULL && curr->rthread)
                curr = curr->right;

            if (curr != NULL)
                curr = curr->right;
        }
    }
    printf("end\n");
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
