#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>

static void shift_keys_right(btreeNode* node, int from)
{
    for (int i = node->num_keys - 1; i >= from; i--)
        node->keys[i + 1] = node->keys[i];
}

static void shift_keys_left(btreeNode* node, int from)
{
    for (int i = from + 1; i < node->num_keys; i++)
        node->keys[i - 1] = node->keys[i];
}

static void shift_children_right(btreeNode* node, int from)
{
    for (int i = node->num_keys; i >= from; i--)
        node->children[i + 1] = node->children[i];
}

static void shift_children_left(btreeNode* node, int from)
{
    for (int i = from + 1; i <= node->num_keys; i++)
        node->children[i - 1] = node->children[i];
}

btreeNode* btree_create_node(int is_leaf)
{
    btreeNode* node = malloc(sizeof(btreeNode));
    if (node == NULL)
        return NULL;
    node->num_keys = 0;
    node->is_leaf = is_leaf;
    for (int i = 0; i < 2 * BTREE_MAX_DEGREE; i++)
        node->children[i] = NULL;
    return node;
}

int btree_search(btreeNode* root, int key)
{
    if (root == NULL)
        return 0;

    int i = 0;
    while (i < root->num_keys && key > root->keys[i])
        i++;

    if (i < root->num_keys && root->keys[i] == key)
        return 1;

    if (root->is_leaf)
        return 0;

    return btree_search(root->children[i], key);
}

static int btree_split_child(btreeNode* parent, int i, btreeNode* child, int t)
{
    btreeNode* new_node = btree_create_node(child->is_leaf);
    if (new_node == NULL)
        return 0;

    new_node->num_keys = t - 1;

    for (int j = 0; j < t - 1; j++)
        new_node->keys[j] = child->keys[j + t];

    if (!child->is_leaf)
    {
        for (int j = 0; j < t; j++)
            new_node->children[j] = child->children[j + t];
    }

    child->num_keys = t - 1;

    shift_children_right(parent, i + 1);

    parent->children[i + 1] = new_node;

    shift_keys_right(parent, i);

    parent->keys[i] = child->keys[t - 1];
    parent->num_keys = parent->num_keys + 1;
    return 1;
}

static int btree_insert_nonfull(btreeNode* node, int key, int t)
{
    int i = node->num_keys - 1;

    if (node->is_leaf)
    {
        int pos = 0;
        while (pos < node->num_keys && node->keys[pos] < key)
            pos++;
        shift_keys_right(node, pos);
        node->keys[pos] = key;
        node->num_keys = node->num_keys + 1;
        return 1;
    }
    else
    {
        while (i >= 0 && node->keys[i] > key)
            i--;
        i++;

        if (node->children[i]->num_keys == 2 * t - 1)
        {
            if (!btree_split_child(node, i, node->children[i], t))
                return 0;
            if (node->keys[i] < key)
                i++;
        }
        return btree_insert_nonfull(node->children[i], key, t);
    }
}

int btree_insert(btreeNode** root_ref, int key, int t)
{
    if (btree_search(*root_ref, key) == 1)
        return 0;

    if (*root_ref == NULL)
    {
        *root_ref = btree_create_node(1);
        if (*root_ref == NULL)
            return -1;
        (*root_ref)->keys[0] = key;
        (*root_ref)->num_keys = 1;
        return 1;
    }

    if ((*root_ref)->num_keys == 2 * t - 1)
    {
        btreeNode* new_root = btree_create_node(0);
        if (new_root == NULL)
            return -1;
        new_root->children[0] = *root_ref;
        if (!btree_split_child(new_root, 0, *root_ref, t))
        {
            free(new_root);
            return -1;
        }
        *root_ref = new_root;
    }

    if (!btree_insert_nonfull(*root_ref, key, t))
        return -1;
    return 1;
}

void btree_traverse(const btreeNode* root)
{
    if (root == NULL)
        return;

    int i;
    for (i = 0; i < root->num_keys; i++)
    {
        if (!root->is_leaf)
            btree_traverse(root->children[i]);
        printf("%d ", root->keys[i]);
    }

    if (!root->is_leaf)
        btree_traverse(root->children[i]);
}

static void btree_borrow_from_prev(btreeNode* node, int idx)
{
    btreeNode* child = node->children[idx];
    btreeNode* sibling = node->children[idx - 1];

    shift_keys_right(child, 0);

    if (!child->is_leaf)
        shift_children_right(child, 0);

    child->keys[0] = node->keys[idx - 1];

    if (!child->is_leaf)
        child->children[0] = sibling->children[sibling->num_keys];

    node->keys[idx - 1] = sibling->keys[sibling->num_keys - 1];
    child->num_keys++;
    sibling->num_keys--;
}

static void btree_borrow_from_next(btreeNode* node, int idx)
{
    btreeNode* child = node->children[idx];
    btreeNode* sibling = node->children[idx + 1];

    child->keys[child->num_keys] = node->keys[idx];

    if (!child->is_leaf)
        child->children[child->num_keys + 1] = sibling->children[0];

    node->keys[idx] = sibling->keys[0];

    shift_keys_left(sibling, 0);

    if (!sibling->is_leaf)
        shift_children_left(sibling, 0);

    child->num_keys++;
    sibling->num_keys--;
}

static void btree_merge(btreeNode* node, int idx, int t)
{
    btreeNode* child = node->children[idx];
    btreeNode* sibling = node->children[idx + 1];

    child->keys[t - 1] = node->keys[idx];

    for (int i = 0; i < sibling->num_keys; i++)
        child->keys[t + i] = sibling->keys[i];

    if (!child->is_leaf)
    {
        for (int i = 0; i <= sibling->num_keys; i++)
            child->children[t + i] = sibling->children[i];
    }

    shift_keys_left(node, idx);
    shift_children_left(node, idx + 1);

    child->num_keys += sibling->num_keys + 1;
    node->num_keys--;

    free(sibling);
}

static void btree_fill(btreeNode* node, int idx, int t)
{
    if (idx != 0 && node->children[idx - 1]->num_keys >= t)
        btree_borrow_from_prev(node, idx);
    else if (idx != node->num_keys && node->children[idx + 1]->num_keys >= t)
        btree_borrow_from_next(node, idx);
    else
    {
        if (idx != node->num_keys)
            btree_merge(node, idx, t);
        else
            btree_merge(node, idx - 1, t);
    }
}

btreeNode* btree_delete(btreeNode* root, int key, int t)
{
    if (root == NULL)
        return NULL;

    int idx = 0;
    while (idx < root->num_keys && root->keys[idx] < key)
        idx++;

    if (idx < root->num_keys && root->keys[idx] == key)
    {
        if (root->is_leaf)
        {
            shift_keys_left(root, idx);
            root->num_keys--;
        }
        else
        {
            if (root->children[idx]->num_keys >= t)
            {
                btreeNode* cur = root->children[idx];
                while (!cur->is_leaf)
                    cur = cur->children[cur->num_keys];
                root->keys[idx] = cur->keys[cur->num_keys - 1];
                root->children[idx] =
                    btree_delete(root->children[idx], cur->keys[cur->num_keys - 1], t);
            }
            else if (root->children[idx + 1]->num_keys >= t)
            {
                btreeNode* cur = root->children[idx + 1];
                while (!cur->is_leaf)
                    cur = cur->children[0];
                root->keys[idx] = cur->keys[0];
                root->children[idx + 1] = btree_delete(root->children[idx + 1], cur->keys[0], t);
            }
            else
            {
                btree_merge(root, idx, t);
                root->children[idx] = btree_delete(root->children[idx], key, t);
            }
        }
    }
    else
    {
        if (root->is_leaf)
            return root;

        int last = (idx == root->num_keys);

        if (root->children[idx]->num_keys < t)
            btree_fill(root, idx, t);

        if (last && idx > root->num_keys)
            root->children[idx - 1] = btree_delete(root->children[idx - 1], key, t);
        else
            root->children[idx] = btree_delete(root->children[idx], key, t);
    }

    if (root->num_keys == 0)
    {
        btreeNode* old_root = root;
        root = root->is_leaf ? NULL : root->children[0];
        free(old_root);
    }

    return root;
}

void btree_destroy(btreeNode* root)
{
    if (root == NULL)
        return;

    if (!root->is_leaf)
    {
        for (int i = 0; i <= root->num_keys; i++)
            btree_destroy(root->children[i]);
    }
    free(root);
}
