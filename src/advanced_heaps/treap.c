#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

/* Create a new Treap node with specified key, value, and priority */
TreapNode* create_treap_node(int key, int value, int priority)
{
    TreapNode* node = (TreapNode*)malloc(sizeof(TreapNode));
    if (node == NULL)
    {
        return NULL;
    }
    node->key = key;
    node->value = value;
    node->priority = priority;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Post-order traversal to destroy the entire Treap and free all nodes */
void destroy_treap(TreapNode* root)
{
    if (root != NULL)
    {
        destroy_treap(root->left);
        destroy_treap(root->right);
        free(root);
    }
}

/* Right rotation to maintain the heap property of priority while keeping BST key order */
static TreapNode* rotate_right(TreapNode* y)
{
    TreapNode* x = y->left;
    TreapNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    return x;
}

/* Left rotation to maintain the heap property of priority while keeping BST key order */
static TreapNode* rotate_left(TreapNode* x)
{
    TreapNode* y = x->right;
    TreapNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    return y;
}

/* Insert a key-value pair into the Treap, maintaining both BST key order and Min-Heap priority
 * order */
TreapNode* treap_insert(TreapNode* root, int key, int value, int priority)
{
    if (root == NULL)
    {
        return create_treap_node(key, value, priority);
    }

    if (key <= root->key)
    {
        TreapNode* temp = treap_insert(root->left, key, value, priority);
        if (temp != NULL || root->left == NULL)
        {
            root->left = temp;
            if (root->left->priority < root->priority)
            {
                root = rotate_right(root);
            }
        }
    }
    else
    {
        TreapNode* temp = treap_insert(root->right, key, value, priority);
        if (temp != NULL || root->right == NULL)
        {
            root->right = temp;
            if (root->right->priority < root->priority)
            {
                root = rotate_left(root);
            }
        }
    }

    return root;
}

/* Delete a node with the specified key from the Treap by rotating it down to a leaf and freeing it
 */
TreapNode* treap_delete(TreapNode* root, int key)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (key < root->key)
    {
        root->left = treap_delete(root->left, key);
    }
    else if (key > root->key)
    {
        root->right = treap_delete(root->right, key);
    }
    else
    {
        // Node found
        if (root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }
        else if (root->left == NULL)
        {
            TreapNode* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            TreapNode* temp = root->left;
            free(root);
            return temp;
        }
        else
        {
            // Both children exist, rotate the node down to maintain heap priority
            if (root->left->priority < root->right->priority)
            {
                root = rotate_right(root);
                root->right = treap_delete(root->right, key);
            }
            else
            {
                root = rotate_left(root);
                root->left = treap_delete(root->left, key);
            }
        }
    }
    return root;
}

/* Search for a node with the specified key using standard BST logic */
TreapNode* treap_find_node(TreapNode* root, int key)
{
    if (root == NULL || root->key == key)
    {
        return root;
    }
    if (key < root->key)
    {
        return treap_find_node(root->left, key);
    }
    return treap_find_node(root->right, key);
}

/* Return the node containing the minimum key (leftmost element in the BST) */
TreapNode* treap_get_min(TreapNode* root)
{
    if (root == NULL)
    {
        return NULL;
    }
    TreapNode* curr = root;
    while (curr->left != NULL)
    {
        curr = curr->left;
    }
    return curr;
}
