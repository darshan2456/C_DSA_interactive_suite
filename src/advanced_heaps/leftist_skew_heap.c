#include "advanced_heaps.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * =========================================================================
 *                           Leftist Heap
 * =========================================================================
 */

/* Creates a single Leftist Heap node */
LeftistNode* create_leftist_node(int key, int value)
{
    LeftistNode* node = (LeftistNode*)malloc(sizeof(LeftistNode));
    if (node == NULL)
    {
        return NULL;
    }
    node->key = key;
    node->value = value;
    node->npl = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Recursively destroys a Leftist Heap */
void destroy_leftist_heap(LeftistNode* root)
{
    if (root == NULL)
    {
        return;
    }
    destroy_leftist_heap(root->left);
    destroy_leftist_heap(root->right);
    free(root);
}

/* Helper function to swap two leftist child pointers */
static void swap_leftist_children(LeftistNode* x)
{
    LeftistNode* temp = x->left;
    x->left = x->right;
    x->right = temp;
}

/* Merges two leftist heaps */
LeftistNode* leftist_heap_merge(LeftistNode* h1, LeftistNode* h2)
{
    if (h1 == h2)
        return h1;
    if (h1 == NULL)
        return h2;
    if (h2 == NULL)
        return h1;

    /* Ensure h1 has the smaller key to maintain heap property */
    if (h1->key > h2->key)
    {
        LeftistNode* temp = h1;
        h1 = h2;
        h2 = temp;
    }

    /* Merge h2 with the right child of h1 */
    h1->right = leftist_heap_merge(h1->right, h2);

    /* Enforce Leftist property: left child npl >= right child npl */
    if (h1->left == NULL || (h1->right != NULL && h1->left->npl < h1->right->npl))
    {
        swap_leftist_children(h1);
    }

    /* Update the Null Path Length of the node */
    if (h1->right == NULL)
    {
        h1->npl = 0;
    }
    else
    {
        h1->npl = h1->right->npl + 1;
    }

    return h1;
}

/* Inserts a new key-value pair into the Leftist Heap */
LeftistNode* leftist_heap_insert(LeftistNode* root, int key, int value)
{
    LeftistNode* new_node = create_leftist_node(key, value);
    if (new_node == NULL)
    {
        return root;
    }
    return leftist_heap_merge(root, new_node);
}

/* Extracts the minimum element from the Leftist Heap */
LeftistNode* leftist_heap_extract_min(LeftistNode* root, int* min_key, int* min_val)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (min_key)
        *min_key = root->key;
    if (min_val)
        *min_val = root->value;

    LeftistNode* left = root->left;
    LeftistNode* right = root->right;
    free(root);

    return leftist_heap_merge(left, right);
}

/*
 * =========================================================================
 *                             Skew Heap
 * =========================================================================
 */

/* Creates a single Skew Heap node */
SkewNode* create_skew_node(int key, int value)
{
    SkewNode* node = (SkewNode*)malloc(sizeof(SkewNode));
    if (node == NULL)
    {
        return NULL;
    }
    node->key = key;
    node->value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Recursively destroys a Skew Heap */
void destroy_skew_heap(SkewNode* root)
{
    if (root == NULL)
    {
        return;
    }
    destroy_skew_heap(root->left);
    destroy_skew_heap(root->right);
    free(root);
}

/* Merges two Skew Heaps */
SkewNode* skew_heap_merge(SkewNode* h1, SkewNode* h2)
{
    if (h1 == h2)
        return h1;
    if (h1 == NULL)
        return h2;
    if (h2 == NULL)
        return h1;

    /* Ensure h1 has the smaller key to maintain heap property */
    if (h1->key > h2->key)
    {
        SkewNode* temp = h1;
        h1 = h2;
        h2 = temp;
    }

    /* Merge h2 with the right child of h1 */
    h1->right = skew_heap_merge(h1->right, h2);

    /* Unconditionally swap left and right children */
    SkewNode* temp = h1->left;
    h1->left = h1->right;
    h1->right = temp;

    return h1;
}

/* Inserts a new key-value pair into the Skew Heap */
SkewNode* skew_heap_insert(SkewNode* root, int key, int value)
{
    SkewNode* new_node = create_skew_node(key, value);
    if (new_node == NULL)
    {
        return root;
    }
    return skew_heap_merge(root, new_node);
}

/* Extracts the minimum element from the Skew Heap */
SkewNode* skew_heap_extract_min(SkewNode* root, int* min_key, int* min_val)
{
    if (root == NULL)
    {
        return NULL;
    }

    if (min_key)
        *min_key = root->key;
    if (min_val)
        *min_val = root->value;

    SkewNode* left = root->left;
    SkewNode* right = root->right;
    free(root);

    return skew_heap_merge(left, right);
}
