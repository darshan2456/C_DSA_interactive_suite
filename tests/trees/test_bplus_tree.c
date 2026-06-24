#include "trees.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

void test_basic_search()
{
    BPlusTree* tree = bplus_tree_create(3);
    assert(tree != NULL);

    int val;
    // Search empty tree
    assert(!bplus_tree_search(tree, 10, &val));

    // Insert and search
    assert(bplus_tree_insert(tree, 10, 100));
    assert(bplus_tree_search(tree, 10, &val));
    assert(val == 100);

    // Duplicate insertion should fail
    assert(!bplus_tree_insert(tree, 10, 200));

    bplus_tree_destroy(tree);
    printf("test_basic_search passed\n");
}

void test_leaf_splits()
{
    BPlusTree* tree = bplus_tree_create(3);

    // Leaf split happens when inserting the 3rd key
    assert(bplus_tree_insert(tree, 10, 100));
    assert(bplus_tree_insert(tree, 20, 200));
    assert(bplus_tree_insert(tree, 30, 300));

    // The root should now be an internal node
    assert(!tree->root->is_leaf);
    assert(tree->root->num_keys == 1);
    assert(tree->root->keys[0] == 30);

    // Left child is leaf containing [10, 20]
    BPlusNode* left = tree->root->children[0];
    assert(left->is_leaf);
    assert(left->num_keys == 2);
    assert(left->keys[0] == 10);
    assert(left->keys[1] == 20);

    // Right child is leaf containing [30]
    BPlusNode* right = tree->root->children[1];
    assert(right->is_leaf);
    assert(right->num_keys == 1);
    assert(right->keys[0] == 30);

    // Check sibling linking
    assert(left->next == right);
    assert(right->prev == left);
    assert(right->next == NULL);
    assert(left->prev == NULL);

    bplus_tree_destroy(tree);
    printf("test_leaf_splits passed\n");
}

void test_internal_splits()
{
    BPlusTree* tree = bplus_tree_create(3);

    // Insert multiple values to force internal node splits
    int keys[] = {10, 20, 30, 5, 15, 25};
    for (int i = 0; i < 6; i++)
    {
        assert(bplus_tree_insert(tree, keys[i], keys[i] * 10));
    }
    // Verify tree height is 3 (root -> level 1 internal -> leaves)
    assert(!tree->root->is_leaf);
    assert(tree->root->num_keys == 1);
    assert(tree->root->keys[0] == 20);

    BPlusNode* lvl1_left = tree->root->children[0];
    BPlusNode* lvl1_right = tree->root->children[1];
    assert(!lvl1_left->is_leaf);
    assert(!lvl1_right->is_leaf);

    assert(lvl1_left->num_keys == 1);
    assert(lvl1_left->keys[0] == 15);

    assert(lvl1_right->num_keys == 1);
    assert(lvl1_right->keys[0] == 30);

    // Sibling traversal check for leaf nodes
    BPlusNode* leaf = lvl1_left->children[0];
    assert(leaf->is_leaf);
    int expected_sorted[] = {5, 10, 15, 20, 25, 30};
    int idx = 0;
    while (leaf)
    {
        for (int i = 0; i < leaf->num_keys; i++)
        {
            assert(leaf->keys[i] == expected_sorted[idx++]);
        }
        leaf = leaf->next;
    }
    assert(idx == 6);

    bplus_tree_destroy(tree);
    printf("test_internal_splits passed\n");
}

void test_leaf_linking()
{
    BPlusTree* tree = bplus_tree_create(4);
    for (int i = 1; i <= 20; i++)
    {
        assert(bplus_tree_insert(tree, i, i * 10));
    }

    // Traverse leaves forwards
    BPlusNode* leaf = tree->root;
    while (leaf && !leaf->is_leaf)
    {
        leaf = leaf->children[0];
    }

    BPlusNode* last = NULL;
    int expected = 1;
    while (leaf)
    {
        for (int i = 0; i < leaf->num_keys; i++)
        {
            assert(leaf->keys[i] == expected);
            expected++;
        }
        last = leaf;
        leaf = leaf->next;
    }
    assert(expected == 21);

    // Traverse leaves backwards
    expected = 20;
    leaf = last;
    while (leaf)
    {
        for (int i = leaf->num_keys - 1; i >= 0; i--)
        {
            assert(leaf->keys[i] == expected);
            expected--;
        }
        leaf = leaf->prev;
    }
    assert(expected == 0);

    bplus_tree_destroy(tree);
    printf("test_leaf_linking passed\n");
}

void test_range_query()
{
    BPlusTree* tree = bplus_tree_create(3);
    for (int i = 1; i <= 10; i++)
    {
        bplus_tree_insert(tree, i, i * 10);
    }

    // Just verifying no crash during range queries
    printf("Range query [3, 7] output: ");
    bplus_tree_range_query(tree, 3, 7);

    printf("Range query [0, 15] output: ");
    bplus_tree_range_query(tree, 0, 15);

    bplus_tree_destroy(tree);
    printf("test_range_query passed\n");
}

void test_borrow_and_merge()
{
    BPlusTree* tree = bplus_tree_create(3);
    for (int i = 10; i <= 50; i += 10)
    {
        bplus_tree_insert(tree, i, i * 10);
    }

    // Initial tree leaves: [10, 20] -> [30] -> [40, 50]
    // Delete 30 -> forces merge/borrow
    assert(bplus_tree_delete(tree, 30));
    int val;
    assert(!bplus_tree_search(tree, 30, &val));

    // Verify other keys still exist
    assert(bplus_tree_search(tree, 10, &val));
    assert(bplus_tree_search(tree, 20, &val));
    assert(bplus_tree_search(tree, 40, &val));
    assert(bplus_tree_search(tree, 50, &val));

    // Delete remaining to force root demotion
    assert(bplus_tree_delete(tree, 10));
    assert(bplus_tree_delete(tree, 20));
    assert(bplus_tree_delete(tree, 40));
    assert(bplus_tree_delete(tree, 50));

    assert(tree->root == NULL);

    bplus_tree_destroy(tree);
    printf("test_borrow_and_merge passed\n");
}

int main()
{
    test_basic_search();
    test_leaf_splits();
    test_internal_splits();
    test_leaf_linking();
    test_range_query();
    test_borrow_and_merge();

    printf("All B+ Tree tests passed successfully!\n");
    return 0;
}
