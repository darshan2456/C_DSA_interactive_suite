#include "mwst_utils.h"
#include "safe_input.h"
#include "trees.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Create an empty B+ Tree */
BPlusTree* bplus_tree_create(int order)
{
    if (order < 3)
        return NULL;
    BPlusTree* tree = malloc(sizeof(BPlusTree));
    if (!tree)
        return NULL;
    tree->root = NULL;
    tree->order = order;
    return tree;
}

/* Destroy the B+ Tree */
void bplus_tree_destroy(BPlusTree* tree)
{
    if (!tree)
        return;
    bplus_node_destroy_recursive(tree->root);
    free(tree);
}

/* Helper: find the leaf node that should contain key */
static BPlusNode* find_leaf(BPlusNode* root, int key)
{
    if (!root)
        return NULL;
    BPlusNode* curr = root;
    while (!curr->is_leaf)
    {
        int idx = 0;
        while (idx < curr->num_keys && key >= curr->keys[idx])
        {
            idx++;
        }
        curr = curr->children[idx];
    }
    return curr;
}

/* Search for a key in the B+ Tree */
bool bplus_tree_search(BPlusTree* tree, int key, int* value_out)
{
    if (!tree || !tree->root)
        return false;
    BPlusNode* leaf = find_leaf(tree->root, key);
    if (!leaf)
        return false;
    for (int i = 0; i < leaf->num_keys; i++)
    {
        if (leaf->keys[i] == key)
        {
            if (value_out)
                *value_out = leaf->values[i];
            return true;
        }
    }
    return false;
}

/* Helper to update index keys in ancestors after deletion */
static void bplus_update_key(BPlusNode* node, int old_key, int new_key)
{
    if (!node || node->is_leaf)
        return;
    for (int i = 0; i < node->num_keys; i++)
    {
        if (node->keys[i] == old_key)
        {
            node->keys[i] = new_key;
            return;
        }
    }
    for (int i = 0; i <= node->num_keys; i++)
    {
        bplus_update_key(node->children[i], old_key, new_key);
    }
}

/* Recursive insert helper */
static BPlusNode* insert_recurse(BPlusTree* tree, BPlusNode* current, int key, int value,
                                 int* promo_key, BPlusNode** promo_node)
{
    if (current->is_leaf)
    {
        for (int i = 0; i < current->num_keys; i++)
        {
            if (current->keys[i] == key)
            {
                *promo_node = NULL;
                return NULL;
            }
        }

        int idx = 0;
        while (idx < current->num_keys && current->keys[idx] < key)
        {
            idx++;
        }
        mwst_shift_keys_right(current->keys, current->num_keys, idx);
        /* shift values right manually since values is int*, not part of mwst */
        for (int i = current->num_keys; i > idx; i--)
            current->values[i] = current->values[i - 1];
        current->keys[idx] = key;
        current->values[idx] = value;
        current->num_keys++;

        if (current->num_keys == tree->order)
        {
            BPlusNode* right = bplus_node_create(tree->order, true);
            if (right == NULL)
            {
                current->num_keys--;
                for (int i = idx; i < current->num_keys; i++)
                {
                    current->keys[i] = current->keys[i + 1];
                    current->values[i] = current->values[i + 1];
                }
                *promo_node = NULL;
                return NULL;
            }
            int left_size = (tree->order + 1) / 2;
            int right_size = tree->order - left_size;

            for (int i = 0; i < right_size; i++)
            {
                right->keys[i] = current->keys[left_size + i];
                right->values[i] = current->values[left_size + i];
            }
            current->num_keys = left_size;
            right->num_keys = right_size;

            right->next = current->next;
            right->prev = current;
            if (current->next)
                current->next->prev = right;
            current->next = right;

            *promo_key = right->keys[0];
            *promo_node = right;
            return right;
        }
        *promo_node = NULL;
        return NULL;
    }
    else
    {
        int idx = 0;
        while (idx < current->num_keys && key >= current->keys[idx])
        {
            idx++;
        }

        BPlusNode* child_split =
            insert_recurse(tree, current->children[idx], key, value, promo_key, promo_node);
        if (child_split)
        {
            int new_key = *promo_key;
            BPlusNode* new_child = *promo_node;

            int ins_idx = 0;
            while (ins_idx < current->num_keys && current->keys[ins_idx] < new_key)
            {
                ins_idx++;
            }
            mwst_shift_keys_right(current->keys, current->num_keys, ins_idx);
            mwst_shift_children_right((void**)current->children, current->num_keys, ins_idx + 1);
            current->keys[ins_idx] = new_key;
            current->children[ins_idx + 1] = new_child;
            current->num_keys++;

            if (current->num_keys == tree->order)
            {
                BPlusNode* right = bplus_node_create(tree->order, false);
                if (right == NULL)
                {
                    current->num_keys--;
                    for (int i = ins_idx; i < current->num_keys; i++)
                    {
                        current->keys[i] = current->keys[i + 1];
                    }
                    for (int i = ins_idx + 1; i <= current->num_keys; i++)
                    {
                        current->children[i] = current->children[i + 1];
                    }
                    *promo_node = NULL;
                    return NULL;
                }
                int left_size = tree->order / 2;
                int right_size = tree->order - left_size - 1;
                int pushed_up_key = current->keys[left_size];

                for (int i = 0; i < right_size; i++)
                    right->keys[i] = current->keys[left_size + 1 + i];
                for (int i = 0; i <= right_size; i++)
                    right->children[i] = current->children[left_size + 1 + i];

                current->num_keys = left_size;
                right->num_keys = right_size;

                *promo_key = pushed_up_key;
                *promo_node = right;
                return right;
            }
        }
        *promo_node = NULL;
        return NULL;
    }
}

/* Insert a key-value pair into the B+ Tree */
bool bplus_tree_insert(BPlusTree* tree, int key, int value)
{
    if (!tree)
        return false;
    if (bplus_tree_search(tree, key, NULL))
        return false;

    if (!tree->root)
    {
        tree->root = bplus_node_create(tree->order, true);
        if (tree->root == NULL)
            return false;
        tree->root->keys[0] = key;
        tree->root->values[0] = value;
        tree->root->num_keys = 1;
        return true;
    }

    int promo_key;
    BPlusNode* promo_node = NULL;
    insert_recurse(tree, tree->root, key, value, &promo_key, &promo_node);

    if (promo_node)
    {
        BPlusNode* new_root = bplus_node_create(tree->order, false);
        if (new_root == NULL)
            return false;
        new_root->keys[0] = promo_key;
        new_root->children[0] = tree->root;
        new_root->children[1] = promo_node;
        new_root->num_keys = 1;
        tree->root = new_root;
    }
    return true;
}

/* Recursive delete helper */
static int delete_recurse(BPlusTree* tree, BPlusNode* current, int key)
{
    if (current->is_leaf)
    {
        int idx = -1;
        for (int i = 0; i < current->num_keys; i++)
        {
            if (current->keys[i] == key)
            {
                idx = i;
                break;
            }
        }
        if (idx == -1)
            return 0;

        int old_first_key = current->keys[0];
        mwst_shift_keys_left(current->keys, current->num_keys, idx);
        for (int i = idx; i < current->num_keys - 1; i++)
            current->values[i] = current->values[i + 1];
        current->num_keys--;

        if (idx == 0 && current->num_keys > 0)
            bplus_update_key(tree->root, old_first_key, current->keys[0]);

        if (current == tree->root)
        {
            if (current->num_keys == 0)
            {
                tree->root = NULL;
                bplus_node_free(current);
            }
            return 1;
        }
        int leaf_min = tree->order / 2;
        return (current->num_keys < leaf_min) ? 2 : 1;
    }
    else
    {
        int idx = 0;
        while (idx < current->num_keys && key >= current->keys[idx])
            idx++;

        int res = delete_recurse(tree, current->children[idx], key);
        if (res != 2)
            return res;

        BPlusNode* child_node = current->children[idx];
        BPlusNode* left_sib = (idx > 0) ? current->children[idx - 1] : NULL;
        BPlusNode* right_sib = (idx < current->num_keys) ? current->children[idx + 1] : NULL;

        if (child_node->is_leaf)
        {
            int leaf_min = tree->order / 2;

            if (left_sib && left_sib->num_keys > leaf_min)
            {
                mwst_shift_keys_right(child_node->keys, child_node->num_keys, 0);
                for (int i = child_node->num_keys; i > 0; i--)
                    child_node->values[i] = child_node->values[i - 1];
                child_node->keys[0] = left_sib->keys[left_sib->num_keys - 1];
                child_node->values[0] = left_sib->values[left_sib->num_keys - 1];
                left_sib->num_keys--;
                child_node->num_keys++;
                current->keys[idx - 1] = child_node->keys[0];
                return 1;
            }

            if (right_sib && right_sib->num_keys > leaf_min)
            {
                child_node->keys[child_node->num_keys] = right_sib->keys[0];
                child_node->values[child_node->num_keys] = right_sib->values[0];
                child_node->num_keys++;
                int old_r_key = right_sib->keys[0];
                mwst_shift_keys_left(right_sib->keys, right_sib->num_keys, 0);
                for (int i = 0; i < right_sib->num_keys - 1; i++)
                    right_sib->values[i] = right_sib->values[i + 1];
                right_sib->num_keys--;
                current->keys[idx] = right_sib->keys[0];
                bplus_update_key(tree->root, old_r_key, right_sib->keys[0]);
                return 1;
            }

            if (left_sib)
            {
                for (int i = 0; i < child_node->num_keys; i++)
                {
                    left_sib->keys[left_sib->num_keys + i] = child_node->keys[i];
                    left_sib->values[left_sib->num_keys + i] = child_node->values[i];
                }
                left_sib->num_keys += child_node->num_keys;
                left_sib->next = child_node->next;
                if (child_node->next)
                    child_node->next->prev = left_sib;

                mwst_shift_keys_left(current->keys, current->num_keys, idx - 1);
                mwst_shift_children_left((void**)current->children, current->num_keys, idx);
                current->num_keys--;
                bplus_node_free(child_node);

                if (current == tree->root && current->num_keys == 0)
                {
                    tree->root = left_sib;
                    bplus_node_free(current);
                    return 1;
                }
                int int_min = (tree->order + 1) / 2 - 1;
                return (current->num_keys < int_min) ? 2 : 1;
            }
            else if (right_sib)
            {
                for (int i = 0; i < right_sib->num_keys; i++)
                {
                    child_node->keys[child_node->num_keys + i] = right_sib->keys[i];
                    child_node->values[child_node->num_keys + i] = right_sib->values[i];
                }
                child_node->num_keys += right_sib->num_keys;
                child_node->next = right_sib->next;
                if (right_sib->next)
                    right_sib->next->prev = child_node;

                mwst_shift_keys_left(current->keys, current->num_keys, idx);
                mwst_shift_children_left((void**)current->children, current->num_keys, idx + 1);
                current->num_keys--;
                bplus_node_free(right_sib);

                if (current == tree->root && current->num_keys == 0)
                {
                    tree->root = child_node;
                    bplus_node_free(current);
                    return 1;
                }
                int int_min = (tree->order + 1) / 2 - 1;
                return (current->num_keys < int_min) ? 2 : 1;
            }
        }
        else
        {
            int int_min = (tree->order + 1) / 2 - 1;

            if (left_sib && left_sib->num_keys > int_min)
            {
                mwst_shift_keys_right(child_node->keys, child_node->num_keys, 0);
                mwst_shift_children_right((void**)child_node->children, child_node->num_keys, 0);
                child_node->keys[0] = current->keys[idx - 1];
                child_node->children[0] = left_sib->children[left_sib->num_keys];
                child_node->num_keys++;
                current->keys[idx - 1] = left_sib->keys[left_sib->num_keys - 1];
                left_sib->num_keys--;
                return 1;
            }

            if (right_sib && right_sib->num_keys > int_min)
            {
                child_node->keys[child_node->num_keys] = current->keys[idx];
                child_node->children[child_node->num_keys + 1] = right_sib->children[0];
                child_node->num_keys++;
                current->keys[idx] = right_sib->keys[0];
                mwst_shift_keys_left(right_sib->keys, right_sib->num_keys, 0);
                mwst_shift_children_left((void**)right_sib->children, right_sib->num_keys, 0);
                right_sib->num_keys--;
                return 1;
            }

            if (left_sib)
            {
                left_sib->keys[left_sib->num_keys] = current->keys[idx - 1];
                left_sib->num_keys++;
                for (int i = 0; i < child_node->num_keys; i++)
                    left_sib->keys[left_sib->num_keys + i] = child_node->keys[i];
                for (int i = 0; i <= child_node->num_keys; i++)
                    left_sib->children[left_sib->num_keys + i] = child_node->children[i];
                left_sib->num_keys += child_node->num_keys;

                mwst_shift_keys_left(current->keys, current->num_keys, idx - 1);
                mwst_shift_children_left((void**)current->children, current->num_keys, idx);
                current->num_keys--;
                bplus_node_free(child_node);

                if (current == tree->root && current->num_keys == 0)
                {
                    tree->root = left_sib;
                    bplus_node_free(current);
                    return 1;
                }
                return (current->num_keys < int_min) ? 2 : 1;
            }
            else if (right_sib)
            {
                child_node->keys[child_node->num_keys] = current->keys[idx];
                child_node->num_keys++;
                for (int i = 0; i < right_sib->num_keys; i++)
                    child_node->keys[child_node->num_keys + i] = right_sib->keys[i];
                for (int i = 0; i <= right_sib->num_keys; i++)
                    child_node->children[child_node->num_keys + i] = right_sib->children[i];
                child_node->num_keys += right_sib->num_keys;

                mwst_shift_keys_left(current->keys, current->num_keys, idx);
                mwst_shift_children_left((void**)current->children, current->num_keys, idx + 1);
                current->num_keys--;
                bplus_node_free(right_sib);

                if (current == tree->root && current->num_keys == 0)
                {
                    tree->root = child_node;
                    bplus_node_free(current);
                    return 1;
                }
                return (current->num_keys < int_min) ? 2 : 1;
            }
        }
        return 1;
    }
}

/* Delete a key from the B+ Tree */
bool bplus_tree_delete(BPlusTree* tree, int key)
{
    if (!tree || !tree->root)
        return false;
    return delete_recurse(tree, tree->root, key) != 0;
}

/* Range queries on B+ Tree */
void bplus_tree_range_query(BPlusTree* tree, int lower, int upper)
{
    if (!tree || !tree->root)
    {
        printf("Tree is empty\n");
        return;
    }
    BPlusNode* leaf = find_leaf(tree->root, lower);
    bool found = false;
    while (leaf)
    {
        for (int i = 0; i < leaf->num_keys; i++)
        {
            if (leaf->keys[i] >= lower && leaf->keys[i] <= upper)
            {
                printf("[%d: %d] ", leaf->keys[i], leaf->values[i]);
                found = true;
            }
            if (leaf->keys[i] > upper)
            {
                leaf = NULL;
                break;
            }
        }
        if (leaf)
            leaf = leaf->next;
    }
    if (!found)
        printf("No records found in range [%d, %d]", lower, upper);
    printf("\n");
}

/* Helper to get the height of the B+ Tree */
static int get_height(BPlusNode* node)
{
    int height = 0;
    while (node)
    {
        height++;
        if (node->is_leaf)
            break;
        node = node->children[0];
    }
    return height;
}

/* Print keys at a specific level */
static void print_level(BPlusNode* node, int current_level, int target_level)
{
    if (!node)
        return;
    if (current_level == target_level)
    {
        printf("[");
        for (int i = 0; i < node->num_keys; i++)
        {
            printf("%d", node->keys[i]);
            if (i < node->num_keys - 1)
                printf(",");
        }
        printf("] ");
        return;
    }
    if (!node->is_leaf)
    {
        for (int i = 0; i <= node->num_keys; i++)
            print_level(node->children[i], current_level + 1, target_level);
    }
}

/* Print visual level-order structure of the B+ Tree */
void bplus_tree_print(BPlusTree* tree)
{
    if (!tree || !tree->root)
    {
        printf("Empty B+ Tree\n");
        return;
    }
    int height = get_height(tree->root);
    for (int i = 1; i <= height; i++)
    {
        printf(i == height ? "Level %d (Leaves): " : "Level %d (Internal): ", i);
        print_level(tree->root, 1, i);
        printf("\n");
    }
    printf("Leaf Linked List: ");
    BPlusNode* leaf = tree->root;
    while (leaf && !leaf->is_leaf)
        leaf = leaf->children[0];
    while (leaf)
    {
        printf("[");
        for (int i = 0; i < leaf->num_keys; i++)
        {
            printf("%d", leaf->keys[i]);
            if (i < leaf->num_keys - 1)
                printf(",");
        }
        printf("]");
        if (leaf->next)
            printf(" -> ");
        leaf = leaf->next;
    }
    printf("\n");
}

/* Interactive Demo dispatcher */
void bplus_tree_demo(void)
{
    int order;
    while (1)
    {
        int status = safe_input_int(
            &order, "\nEnter B+ Tree order (minimum 3, maximum 10), enter '-1' to exit: ", 3, 10);
        if (status == INPUT_EXIT_SIGNAL)
            return;
        if (status == 0)
            continue;
        break;
    }

    BPlusTree* tree = bplus_tree_create(order);
    if (!tree)
    {
        printf("Failed to create B+ Tree\n");
        return;
    }

    while (1)
    {
        int choice;
        int status = safe_input_int(&choice,
                                    "\n--- B+ Tree Demo Menu ---\n"
                                    "1. Insert key-value pair\n"
                                    "2. Delete key\n"
                                    "3. Search key\n"
                                    "4. Range Query\n"
                                    "5. Print Tree Structure\n"
                                    "Enter choice: ",
                                    1, 5);
        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("Exiting B+ Tree demo...\n");
            bplus_tree_destroy(tree);
            return;
        }
        if (status == 0)
            continue;

        switch (choice)
        {
            case 1:
            {
                int key, val;
                while (1)
                {
                    int s =
                        safe_input_int(&key, "Enter key to insert (positive integer): ", 1, 10000);
                    if (s == INPUT_EXIT_SIGNAL)
                        break;
                    if (s == 0)
                        continue;
                    bool cancelled = false;
                    while (1)
                    {
                        int s2 = safe_input_int(
                            &val, "Enter corresponding value (positive integer): ", 1, 10000);
                        if (s2 == INPUT_EXIT_SIGNAL)
                        {
                            cancelled = true;
                            break;
                        }
                        if (s2 == 0)
                            continue;
                        break;
                    }
                    if (cancelled)
                        break;
                    if (bplus_tree_insert(tree, key, val))
                        printf("Successfully inserted [%d: %d]\n", key, val);
                    else
                        printf("Insertion failed! Key %d may already exist.\n", key);
                    break;
                }
                break;
            }
            case 2:
            {
                int key;
                while (1)
                {
                    int s = safe_input_int(&key, "Enter key to delete: ", 1, 10000);
                    if (s == INPUT_EXIT_SIGNAL)
                        break;
                    if (s == 0)
                        continue;
                    if (bplus_tree_delete(tree, key))
                        printf("Successfully deleted key %d\n", key);
                    else
                        printf("Key %d not found in tree.\n", key);
                    break;
                }
                break;
            }
            case 3:
            {
                int key, val;
                while (1)
                {
                    int s = safe_input_int(&key, "Enter key to search: ", 1, 10000);
                    if (s == INPUT_EXIT_SIGNAL)
                        break;
                    if (s == 0)
                        continue;
                    if (bplus_tree_search(tree, key, &val))
                        printf("Found key %d with value: %d\n", key, val);
                    else
                        printf("Key %d not found.\n", key);
                    break;
                }
                break;
            }
            case 4:
            {
                int lower, upper;
                while (1)
                {
                    int s = safe_input_int(&lower, "Enter lower bound key: ", 1, 10000);
                    if (s == INPUT_EXIT_SIGNAL)
                        break;
                    if (s == 0)
                        continue;
                    bool cancelled = false;
                    while (1)
                    {
                        int s2 = safe_input_int(&upper, "Enter upper bound key: ", lower, 10000);
                        if (s2 == INPUT_EXIT_SIGNAL)
                        {
                            cancelled = true;
                            break;
                        }
                        if (s2 == 0)
                            continue;
                        break;
                    }
                    if (cancelled)
                        break;
                    printf("Range query results for [%d, %d]:\n", lower, upper);
                    bplus_tree_range_query(tree, lower, upper);
                    break;
                }
                break;
            }
            case 5:
            {
                bplus_tree_print(tree);
                break;
            }
        }
    }
}
