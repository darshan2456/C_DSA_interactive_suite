#include "avl_tree.h"
#include <assert.h>
#include <stdio.h>

// recompute height independently to verify the stored height is correct
static int recompute_height(const avlNode* root)
{
    if (root == NULL)
        return 0;
    int lh = recompute_height(root->left);
    int rh = recompute_height(root->right);
    return 1 + ((lh > rh) ? lh : rh);
}

// checks the AVL invariant (|balance factor| <= 1) for every node
static int is_avl_valid(const avlNode* root)
{
    if (root == NULL)
        return 1;
    int bf = recompute_height(root->left) - recompute_height(root->right);
    if (bf < -1 || bf > 1)
        return 0;
    return is_avl_valid(root->left) && is_avl_valid(root->right);
}

void test_insert_basic()
{
    avlNode* root = NULL;

    assert(avl_insert(&root, 10) == 1);
    assert(avl_insert(&root, 20) == 1);
    assert(avl_insert(&root, 30) == 1);

    assert(avl_countnodes(root) == 3);
    assert(is_avl_valid(root));

    destroy_avl(root);
    printf("AVL insert basic test passed\n");
}

void test_duplicate_insert()
{
    avlNode* root = NULL;

    assert(avl_insert(&root, 10) == 1);
    assert(avl_insert(&root, 10) == 0);
    assert(avl_countnodes(root) == 1);

    destroy_avl(root);
    printf("AVL duplicate insert test passed\n");
}

void test_ll_rotation()
{
    avlNode* root = NULL;
    avl_insert(&root, 30);
    avl_insert(&root, 20);
    avl_insert(&root, 10);

    assert(root->data == 20);
    assert(root->left->data == 10);
    assert(root->right->data == 30);
    assert(avl_height(root) == 2);
    assert(is_avl_valid(root));

    destroy_avl(root);
    printf("AVL LL rotation test passed\n");
}

void test_rr_rotation()
{
    avlNode* root = NULL;
    avl_insert(&root, 10);
    avl_insert(&root, 20);
    avl_insert(&root, 30);

    assert(root->data == 20);
    assert(root->left->data == 10);
    assert(root->right->data == 30);
    assert(avl_height(root) == 2);
    assert(is_avl_valid(root));

    destroy_avl(root);
    printf("AVL RR rotation test passed\n");
}

void test_lr_rotation()
{
    avlNode* root = NULL;
    avl_insert(&root, 30);
    avl_insert(&root, 10);
    avl_insert(&root, 20);

    assert(root->data == 20);
    assert(root->left->data == 10);
    assert(root->right->data == 30);
    assert(is_avl_valid(root));

    destroy_avl(root);
    printf("AVL LR rotation test passed\n");
}

void test_rl_rotation()
{
    avlNode* root = NULL;
    avl_insert(&root, 10);
    avl_insert(&root, 30);
    avl_insert(&root, 20);

    assert(root->data == 20);
    assert(root->left->data == 10);
    assert(root->right->data == 30);
    assert(is_avl_valid(root));

    destroy_avl(root);
    printf("AVL RL rotation test passed\n");
}

void test_stress_balance()
{
    avlNode* root = NULL;
    for (int i = 1; i <= 100; i++)
        assert(avl_insert(&root, i) == 1);

    assert(avl_countnodes(root) == 100);
    assert(is_avl_valid(root));
    assert(avl_height(root) <= 10);

    destroy_avl(root);
    printf("AVL stress balance test passed\n");
}

void test_search()
{
    avlNode* root = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++)
        avl_insert(&root, values[i]);

    assert(avl_search(root, 40) == 1);
    assert(avl_search(root, 80) == 1);
    assert(avl_search(root, 50) == 1);
    assert(avl_search(root, 99) == 0);
    assert(avl_search(NULL, 10) == 0);

    destroy_avl(root);
    printf("AVL search test passed\n");
}

void test_delete_leaf()
{
    avlNode* root = NULL;
    avl_insert(&root, 20);
    avl_insert(&root, 10);
    avl_insert(&root, 30);

    assert(avl_delete(&root, 10) == 1);
    assert(avl_search(root, 10) == 0);
    assert(avl_countnodes(root) == 2);
    assert(is_avl_valid(root));

    destroy_avl(root);
    printf("AVL delete leaf test passed\n");
}

void test_delete_two_children()
{
    avlNode* root = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    for (int i = 0; i < 7; i++)
        avl_insert(&root, values[i]);

    assert(avl_delete(&root, 50) == 1);
    assert(avl_search(root, 50) == 0);
    assert(avl_countnodes(root) == 6);
    assert(is_avl_valid(root));

    destroy_avl(root);
    printf("AVL delete with two children test passed\n");
}

void test_delete_not_found()
{
    avlNode* root = NULL;
    avl_insert(&root, 10);

    assert(avl_delete(&root, 99) == 0);
    assert(avl_countnodes(root) == 1);

    destroy_avl(root);
    printf("AVL delete not found test passed\n");
}

void test_delete_triggers_rebalance()
{
    avlNode* root = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10};
    for (int i = 0; i < 8; i++)
        avl_insert(&root, values[i]);

    assert(avl_delete(&root, 70) == 1);
    assert(is_avl_valid(root));
    assert(avl_delete(&root, 60) == 1);
    assert(is_avl_valid(root));
    assert(avl_delete(&root, 80) == 1);
    assert(is_avl_valid(root));

    destroy_avl(root);
    printf("AVL delete triggers rebalance test passed\n");
}

void test_empty_tree()
{
    avlNode* root = NULL;

    assert(avl_countnodes(root) == 0);
    assert(avl_height(root) == 0);
    assert(avl_balance_factor(root) == 0);
    assert(avl_search(root, 10) == 0);
    assert(avl_delete(&root, 10) == 0);

    destroy_avl(root);
    printf("AVL empty tree test passed\n");
}

int main()
{
    test_insert_basic();
    test_duplicate_insert();
    test_ll_rotation();
    test_rr_rotation();
    test_lr_rotation();
    test_rl_rotation();
    test_stress_balance();
    test_search();
    test_delete_leaf();
    test_delete_two_children();
    test_delete_not_found();
    test_delete_triggers_rebalance();
    test_empty_tree();

    printf("All AVL tests passed\n");
    return 0;
}