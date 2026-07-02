#include "advanced_heaps.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* Helper function to check if the binary search tree property is satisfied */
static bool is_bst(TreapNode* root, int min_val, int max_val)
{
    if (root == NULL)
        return true;
    if (root->key < min_val || root->key > max_val)
        return false;
    return is_bst(root->left, min_val, root->key) && is_bst(root->right, root->key, max_val);
}

/* Helper function to check if the min-heap priority property is satisfied */
static bool is_heap(TreapNode* root)
{
    if (root == NULL)
        return true;
    if (root->left != NULL && root->left->priority < root->priority)
        return false;
    if (root->right != NULL && root->right->priority < root->priority)
        return false;
    return is_heap(root->left) && is_heap(root->right);
}

void test_treap_insert_properties()
{
    printf("Running test_treap_insert_properties...\n");
    TreapNode* root = NULL;

    // Keys are values to order by, priorities are assigned specifically to test rotations
    root = treap_insert(root, 50, 500, 10);
    root = treap_insert(root, 30, 300, 20);
    root = treap_insert(root, 70, 700, 5); // Will trigger left rotation to make 70 the parent of 50

    assert(root != NULL);
    assert(is_bst(root, -10000, 10000));
    assert(is_heap(root));

    // Minimum key is 30
    TreapNode* min_node = treap_get_min(root);
    assert(min_node != NULL);
    assert(min_node->key == 30);

    destroy_treap(root);
    printf("test_treap_insert_properties passed.\n");
}

void test_treap_delete()
{
    printf("Running test_treap_delete...\n");
    TreapNode* root = NULL;

    root = treap_insert(root, 50, 500, 15);
    root = treap_insert(root, 30, 300, 30);
    root = treap_insert(root, 70, 700, 10);
    root = treap_insert(root, 20, 200, 45);
    root = treap_insert(root, 40, 400, 25);

    assert(is_bst(root, -10000, 10000));
    assert(is_heap(root));

    // Find node 30
    TreapNode* found = treap_find_node(root, 30);
    assert(found != NULL);
    assert(found->value == 300);

    // Delete node 30
    root = treap_delete(root, 30);
    assert(is_bst(root, -10000, 10000));
    assert(is_heap(root));

    // Verify it is deleted
    assert(treap_find_node(root, 30) == NULL);

    destroy_treap(root);
    printf("test_treap_delete passed.\n");
}

int main()
{
    test_treap_insert_properties();
    test_treap_delete();
    printf("All Treap tests passed!\n");
    return 0;
}
