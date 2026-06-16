#include "mwst_utils.h"
#include <stdbool.h>
#include <stdlib.h>

/* Shift keys right from index 'from' to make room for a new key */
void mwst_shift_keys_right(int* keys, int num_keys, int from)
{
    for (int i = num_keys - 1; i >= from; i--)
        keys[i + 1] = keys[i];
}

/* Shift keys left from index 'from' to fill a deleted key */
void mwst_shift_keys_left(int* keys, int num_keys, int from)
{
    for (int i = from + 1; i < num_keys; i++)
        keys[i - 1] = keys[i];
}

/* Shift child pointers right from index 'from' */
void mwst_shift_children_right(void** children, int num_keys, int from)
{
    for (int i = num_keys; i >= from; i--)
        children[i + 1] = children[i];
}

/* Shift child pointers left from index 'from' */
void mwst_shift_children_left(void** children, int num_keys, int from)
{
    for (int i = from + 1; i <= num_keys; i++)
        children[i - 1] = children[i];
}

/* Allocate a new B+ Tree node */
BPlusNode* bplus_node_create(int order, bool is_leaf)
{
    BPlusNode* node = malloc(sizeof(BPlusNode));
    if (!node)
        return NULL;
    node->is_leaf = is_leaf;
    node->num_keys = 0;

    node->keys = calloc(order, sizeof(int));
    if (!node->keys)
    {
        free(node);
        return NULL;
    }

    if (is_leaf)
    {
        node->values = calloc(order, sizeof(int));
        if (!node->values)
        {
            free(node->keys);
            free(node);
            return NULL;
        }
        node->children = NULL;
    }
    else
    {
        node->values = NULL;
        node->children = calloc(order + 1, sizeof(BPlusNode*));
        if (!node->children)
        {
            free(node->keys);
            free(node);
            return NULL;
        }
    }
    node->next = NULL;
    node->prev = NULL;
    return node;
}

/* Free a single B+ Tree node (does not touch children) */
void bplus_node_free(BPlusNode* node)
{
    if (!node)
        return;
    if (node->keys)
        free(node->keys);
    if (node->values)
        free(node->values);
    if (node->children)
        free(node->children);
    free(node);
}

/* Recursively destroy a B+ Tree node and all its descendants */
void bplus_node_destroy_recursive(BPlusNode* node)
{
    if (!node)
        return;
    if (!node->is_leaf)
    {
        for (int i = 0; i <= node->num_keys; i++)
            bplus_node_destroy_recursive(node->children[i]);
    }
    bplus_node_free(node);
}

/* Validate that keys in a node are in strictly ascending order */
bool bplus_validate_key_ordering(BPlusNode* node)
{
    if (!node)
        return true;
    for (int i = 0; i < node->num_keys - 1; i++)
    {
        if (node->keys[i] >= node->keys[i + 1])
            return false;
    }
    return true;
}

/* Validate that an internal node has exactly num_keys + 1 children */
bool bplus_validate_child_count(BPlusNode* node)
{
    if (!node || node->is_leaf)
        return true;
    for (int i = 0; i <= node->num_keys; i++)
    {
        if (!node->children[i])
            return false;
    }
    return true;
}

/* Validate that leaf linked list prev/next pointers are consistent */
bool bplus_validate_leaf_links(BPlusNode* node)
{
    if (!node)
        return true;
    if (node->next && node->next->prev != node)
        return false;
    if (node->prev && node->prev->next != node)
        return false;
    return true;
}
