#include "trees.h"
#include <assert.h>
#include <stdio.h>

void test_avl_init()
{
    avlNode* root = NULL;
    assert(avl_height(root) == 0);
    assert(avl_balance_factor(root) == 0);

    assert(avl_insert(&root, 10) == 1);
    assert(avl_height(root) == 1);
    assert(avl_balance_factor(root) == 0);

    destroy_avl(root);
    printf("AVL init test passed\n");
}

void test_avl_rotations()
{
    // Left-Left (LL) Case -> Right Rotation
    avlNode* root1 = NULL;
    avl_insert(&root1, 30);
    avl_insert(&root1, 20);
    avl_insert(&root1, 10);
    assert(root1->data == 20);
    assert(root1->left->data == 10);
    assert(root1->right->data == 30);
    assert(avl_height(root1) == 2);
    destroy_avl(root1);

    // Right-Right (RR) Case -> Left Rotation
    avlNode* root2 = NULL;
    avl_insert(&root2, 10);
    avl_insert(&root2, 20);
    avl_insert(&root2, 30);
    assert(root2->data == 20);
    assert(root2->left->data == 10);
    assert(root2->right->data == 30);
    assert(avl_height(root2) == 2);
    destroy_avl(root2);

    // Left-Right (LR) Case -> Left then Right Rotation
    avlNode* root3 = NULL;
    avl_insert(&root3, 30);
    avl_insert(&root3, 10);
    avl_insert(&root3, 20);
    assert(root3->data == 20);
    assert(root3->left->data == 10);
    assert(root3->right->data == 30);
    assert(avl_height(root3) == 2);
    destroy_avl(root3);

    // Right-Left (RL) Case -> Right then Left Rotation
    avlNode* root4 = NULL;
    avl_insert(&root4, 10);
    avl_insert(&root4, 30);
    avl_insert(&root4, 20);
    assert(root4->data == 20);
    assert(root4->left->data == 10);
    assert(root4->right->data == 30);
    assert(avl_height(root4) == 2);
    destroy_avl(root4);

    printf("AVL rotation tests passed\n");
}

void test_avl_deletion()
{
    avlNode* root = NULL;
    avl_insert(&root, 9);
    avl_insert(&root, 5);
    avl_insert(&root, 10);
    avl_insert(&root, 0);
    avl_insert(&root, 6);
    avl_insert(&root, 11);
    avl_insert(&root, -1);
    avl_insert(&root, 1);
    avl_insert(&root, 2);

    /* The constructed AVL Tree would be
            9
           /  \
          1    10
        /  \     \
       0    5     11
      /    / \
     -1   2   6
    */

    // Deletion of node 10 should trigger LL rotation at root (9)
    assert(avl_delete(&root, 10) == 1);
    assert(avl_balance_factor(root) >= -1 && avl_balance_factor(root) <= 1);

    destroy_avl(root);
    printf("AVL deletion test passed\n");
}

int main()
{
    test_avl_init();
    test_avl_rotations();
    test_avl_deletion();

    printf("All AVL Tree tests passed\n");
    return 0;
}
