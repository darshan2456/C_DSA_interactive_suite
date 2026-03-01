#include <assert.h>
#include <stdio.h>
#include "bst.h"

void test_insert_and_count() {
    bstNode* root = NULL;

    assert(bst_insert(&root, 10) == 1);
    assert(bst_insert(&root, 5) == 1);
    assert(bst_insert(&root, 15) == 1);

    assert(countnodes(root) == 3);

    destroy_bst(root);
}

void test_duplicate_insert() {
    bstNode* root = NULL;

    assert(bst_insert(&root, 10) == 1);
    assert(bst_insert(&root, 10) == 0);  // duplicate

    assert(countnodes(root) == 1);

    destroy_bst(root);
}

void test_height_balanced() {
    bstNode* root = NULL;

    bst_insert(&root, 10);
    bst_insert(&root, 5);
    bst_insert(&root, 15);

    // Balanced tree of 3 nodes
    assert(tree_height(root) == 2);

    destroy_bst(root);
}

void test_height_skewed() {
    bstNode* root = NULL;

    bst_insert(&root, 1);
    bst_insert(&root, 2);
    bst_insert(&root, 3);
    bst_insert(&root, 4);

    // Completely right-skewed
    assert(tree_height(root) == 4);

    destroy_bst(root);
}

void test_empty_tree() {
    bstNode* root = NULL;

    assert(countnodes(root) == 0);
    assert(tree_height(root) == 0);

    destroy_bst(root);
}

int main() {
    test_insert_and_count();
    test_duplicate_insert();
    test_height_balanced();
    test_height_skewed();
    test_empty_tree();

    printf("All BST tests passed\n");
    return 0;
}