#include "trees.h"
#include <assert.h>
#include <stdio.h>

void test_insert_and_count()
{
    bstNode* root = NULL;

    assert(bst_insert(&root, 10) == 1);
    assert(bst_insert(&root, 5) == 1);
    assert(bst_insert(&root, 15) == 1);
    assert(bst_insert(&root, 19) == 1);

    assert(countnodes(root) == 4);

    destroy_bst(root);

    printf("BST insert and count test passed\n");
}

void test_duplicate_insert()
{
    bstNode* root = NULL;

    assert(bst_insert(&root, 10) == 1);
    assert(bst_insert(&root, 10) == 0);

    assert(countnodes(root) == 1);

    destroy_bst(root);

    printf("BST duplicate insert test passed\n");
}

void test_height_balanced()
{
    bstNode* root = NULL;

    bst_insert(&root, 10);
    bst_insert(&root, 5);
    bst_insert(&root, 15);

    assert(tree_height(root) == 2);

    destroy_bst(root);

    printf("BST balanced height test passed\n");
}

void test_height_skewed()
{
    bstNode* root = NULL;

    bst_insert(&root, 1);
    bst_insert(&root, 2);
    bst_insert(&root, 3);
    bst_insert(&root, 4);

    assert(tree_height(root) == 4);

    destroy_bst(root);

    printf("BST skewed height test passed\n");
}

void test_empty_tree()
{
    bstNode* root = NULL;

    assert(countnodes(root) == 0);
    assert(tree_height(root) == 0);

    destroy_bst(root);

    printf("BST empty tree test passed\n");
}

void test_delete_leaf()
{
    bstNode* root = NULL;

    bst_insert(&root, 10);
    bst_insert(&root, 5);
    bst_insert(&root, 15);

    root = bst_delete(root, 5);

    assert(countnodes(root) == 2);
    assert(bst_insert(&root, 5) == 1);

    destroy_bst(root);

    printf("BST delete leaf test passed\n");
}

void test_delete_single_child()
{
    bstNode* root = NULL;

    bst_insert(&root, 10);
    bst_insert(&root, 5);
    bst_insert(&root, 3);

    root = bst_delete(root, 5);

    assert(countnodes(root) == 2);
    assert(bst_insert(&root, 5) == 1);

    destroy_bst(root);

    printf("BST delete single child test passed\n");
}

void test_delete_two_children()
{
    bstNode* root = NULL;

    bst_insert(&root, 10);
    bst_insert(&root, 5);
    bst_insert(&root, 15);
    bst_insert(&root, 3);
    bst_insert(&root, 7);

    root = bst_delete(root, 5);

    assert(countnodes(root) == 4);
    assert(bst_insert(&root, 5) == 1);

    destroy_bst(root);

    printf("BST delete two children test passed\n");
}

void test_delete_nonexistent()
{
    bstNode* root = NULL;

    bst_insert(&root, 10);
    bst_insert(&root, 5);

    root = bst_delete(root, 99);

    assert(countnodes(root) == 2);

    destroy_bst(root);

    printf("BST delete nonexistent node test passed\n");
}

int main()
{
    test_insert_and_count();
    test_duplicate_insert();
    test_height_balanced();
    test_height_skewed();
    test_empty_tree();
    test_delete_leaf();
    test_delete_single_child();
    test_delete_two_children();
    test_delete_nonexistent();

    printf("All BST tests passed\n");
    return 0;
}