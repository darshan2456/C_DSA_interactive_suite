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

static void btree_split_child(btreeNode* parent, int i, btreeNode* child, int t)
{
    btreeNode* new_node = btree_create_node(child->is_leaf);
    if (new_node == NULL)
        return;

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
}

static void btree_insert_nonfull(btreeNode* node, int key, int t)
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
    }
    else
    {
        while (i >= 0 && node->keys[i] > key)
            i--;
        i++;

        if (node->children[i]->num_keys == 2 * t - 1)
        {
            btree_split_child(node, i, node->children[i], t);
            if (node->keys[i] < key)
                i++;
        }
        btree_insert_nonfull(node->children[i], key, t);
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
        btree_split_child(new_root, 0, *root_ref, t);
        *root_ref = new_root;
    }

    btree_insert_nonfull(*root_ref, key, t);
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

void btree_demo(void)
{
    while (1)
    {
        btreeNode* root = NULL;
        int t;
        int t_status;

        printf("\n");
        t_status = safe_input_int(&t,
                                  "enter minimum degree t for B-Tree (between 2 and 10), "
                                  "enter '-1' to exit: ",
                                  2, 10);

        if (t_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting B-Tree demo\n");
            btree_destroy(root);
            return;
        }
        if (t_status == 0)
            continue;

        int total_keys;
        int total_keys_status;

        total_keys_status = safe_input_int(&total_keys,
                                           "enter total number of keys to insert "
                                           "(between 1 and 100), enter '-1' to exit: ",
                                           1, 100);

        if (total_keys_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting B-Tree demo\n");
            btree_destroy(root);
            return;
        }
        if (total_keys_status == 0)
            continue;

        int i = 1;
        int remaining = total_keys;
        while (remaining > 0)
        {
            int key;
            int key_status;
            printf("\nenter value of key %d - ", i);
            key_status = safe_input_int(&key, NULL, 1, 1000);

            if (key_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting B-Tree demo\n");
                btree_destroy(root);
                return;
            }
            if (key_status == 0)
                continue;

            int result = btree_insert(&root, key, t);
            if (result == 0)
            {
                printf("\nduplicate key. only unique keys please");
                continue;
            }
            if (result == -1)
            {
                printf("\ncould not insert key due to malloc failure. try again\n");
                continue;
            }
            i++;
            remaining--;
        }

        printf("\nB-Tree inorder traversal: ");
        btree_traverse(root);
        printf("\n");

        while (1)
        {
            int op;
            int op_status;

            op_status = safe_input_int(&op,
                                       "\nenter '1' to search, '2' to insert, "
                                       "'3' to delete, '-1' to exit: ",
                                       1, 3);

            if (op_status == INPUT_EXIT_SIGNAL)
            {
                btree_destroy(root);
                break;
            }
            if (op_status == 0)
                continue;

            if (op == 1)
            {
                int search_key;
                int search_status;
                while (1)
                {
                    search_status = safe_input_int(&search_key,
                                                   "\nenter key to search (between 1 and 1000), "
                                                   "enter '-1' to exit: ",
                                                   1, 1000);
                    if (search_status == INPUT_EXIT_SIGNAL)
                    {
                        btree_destroy(root);
                        return;
                    }
                    if (search_status == 0)
                        continue;
                    break;
                }
                if (btree_search(root, search_key))
                    printf("\nkey %d found in B-Tree\n", search_key);
                else
                    printf("\nkey %d not found in B-Tree\n", search_key);
            }
            else if (op == 2)
            {
                int new_key;
                int new_key_status;
                while (1)
                {
                    new_key_status = safe_input_int(&new_key,
                                                    "\nenter key to insert (between 1 and 1000), "
                                                    "enter '-1' to exit: ",
                                                    1, 1000);
                    if (new_key_status == INPUT_EXIT_SIGNAL)
                    {
                        btree_destroy(root);
                        return;
                    }
                    if (new_key_status == 0)
                        continue;
                    break;
                }
                int result = btree_insert(&root, new_key, t);
                if (result == 0)
                    printf("\nduplicate key. only unique keys please\n");
                else if (result == -1)
                    printf("\ncould not insert due to malloc failure\n");
                else
                {
                    printf("\nkey inserted. updated traversal: ");
                    btree_traverse(root);
                    printf("\n");
                }
            }
            else if (op == 3)
            {
                int del_key;
                int del_status;
                while (1)
                {
                    del_status = safe_input_int(&del_key,
                                                "\nenter key to delete (between 1 and 1000), "
                                                "enter '-1' to exit: ",
                                                1, 1000);
                    if (del_status == INPUT_EXIT_SIGNAL)
                    {
                        btree_destroy(root);
                        return;
                    }
                    if (del_status == 0)
                        continue;
                    break;
                }
                root = btree_delete(root, del_key, t);
                printf("\nkey deleted. updated traversal: ");
                btree_traverse(root);
                printf("\n");
            }
        }
    }
}