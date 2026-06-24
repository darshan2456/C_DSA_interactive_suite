#include "trees.h"
#include <assert.h>
#include <stdio.h>

static int btree_validate_keys(const btreeNode* node)
{
    if (node == NULL)
        return 1;

    for (int i = 0; i < node->num_keys - 1; i++)
    {
        if (node->keys[i] >= node->keys[i + 1])
            return 0;
    }

    if (!node->is_leaf)
    {
        for (int i = 0; i <= node->num_keys; i++)
        {
            if (!btree_validate_keys(node->children[i]))
                return 0;
        }
    }

    return 1;
}

static int btree_validate_invariants(const btreeNode* node, int t, int is_root)
{
    if (node == NULL)
        return 1;

    if (node->num_keys > 2 * t - 1)
        return 0;

    if (!is_root && node->num_keys < t - 1)
        return 0;

    if (!node->is_leaf)
    {
        for (int i = 0; i <= node->num_keys; i++)
        {
            if (node->children[i] == NULL)
                return 0;
            if (!btree_validate_invariants(node->children[i], t, 0))
                return 0;
        }
    }

    return 1;
}

void test_insert_and_search()
{
    btreeNode* root = NULL;
    int t = 2;

    assert(btree_insert(&root, 10, t) == 1);
    assert(btree_insert(&root, 20, t) == 1);
    assert(btree_insert(&root, 30, t) == 1);

    assert(btree_search(root, 10) == 1);
    assert(btree_search(root, 20) == 1);
    assert(btree_search(root, 30) == 1);
    assert(btree_search(root, 99) == 0);

    btree_destroy(root);
    printf("btree insert and search test passed\n");
}

void test_duplicate_insert()
{
    btreeNode* root = NULL;
    int t = 2;

    assert(btree_insert(&root, 10, t) == 1);
    assert(btree_insert(&root, 10, t) == 0);

    btree_destroy(root);
    printf("btree duplicate insert test passed\n");
}

void test_empty_tree()
{
    btreeNode* root = NULL;

    assert(btree_search(root, 10) == 0);

    btree_destroy(root);
    printf("btree empty tree test passed\n");
}

void test_split()
{
    btreeNode* root = NULL;
    int t = 2;

    assert(btree_insert(&root, 10, t) == 1);
    assert(btree_insert(&root, 20, t) == 1);
    assert(btree_insert(&root, 30, t) == 1);
    assert(btree_insert(&root, 40, t) == 1);
    assert(btree_insert(&root, 50, t) == 1);

    assert(btree_search(root, 10) == 1);
    assert(btree_search(root, 30) == 1);
    assert(btree_search(root, 50) == 1);

    btree_destroy(root);
    printf("btree split test passed\n");
}

void test_delete_leaf_key()
{
    btreeNode* root = NULL;
    int t = 2;

    btree_insert(&root, 10, t);
    btree_insert(&root, 20, t);
    btree_insert(&root, 30, t);

    root = btree_delete(root, 20, t);

    assert(btree_search(root, 20) == 0);
    assert(btree_search(root, 10) == 1);
    assert(btree_search(root, 30) == 1);

    btree_destroy(root);
    printf("btree delete leaf key test passed\n");
}

void test_delete_nonexistent()
{
    btreeNode* root = NULL;
    int t = 2;

    btree_insert(&root, 10, t);
    btree_insert(&root, 20, t);

    root = btree_delete(root, 99, t);

    assert(btree_search(root, 10) == 1);
    assert(btree_search(root, 20) == 1);

    btree_destroy(root);
    printf("btree delete nonexistent key test passed\n");
}

void test_delete_internal_key()
{
    btreeNode* root = NULL;
    int t = 2;

    btree_insert(&root, 10, t);
    btree_insert(&root, 20, t);
    btree_insert(&root, 30, t);
    btree_insert(&root, 40, t);
    btree_insert(&root, 50, t);

    root = btree_delete(root, 20, t);

    assert(btree_search(root, 20) == 0);
    assert(btree_search(root, 10) == 1);
    assert(btree_search(root, 30) == 1);
    assert(btree_search(root, 40) == 1);
    assert(btree_search(root, 50) == 1);

    btree_destroy(root);
    printf("btree delete internal key test passed\n");
}

void test_traverse()
{
    btreeNode* root = NULL;
    int t = 2;

    btree_insert(&root, 10, t);
    btree_insert(&root, 20, t);
    btree_insert(&root, 30, t);

    printf("btree traversal output: ");
    btree_traverse(root);
    printf("\n");

    btree_destroy(root);
    printf("btree traverse test passed\n");
}

void test_stress()
{
    btreeNode* root = NULL;
    int t = 3;

    for (int i = 1; i <= 10000; i++)
        assert(btree_insert(&root, i, t) == 1);

    for (int i = 1; i <= 10000; i++)
        assert(btree_search(root, i) == 1);

    for (int i = 1; i <= 10000; i += 2)
        root = btree_delete(root, i, t);

    for (int i = 1; i <= 10000; i += 2)
        assert(btree_search(root, i) == 0);

    for (int i = 2; i <= 10000; i += 2)
        assert(btree_search(root, i) == 1);

    assert(btree_validate_keys(root));
    assert(btree_validate_invariants(root, t, 1));
    btree_destroy(root);
    printf("btree stress test passed\n");
}

int main()
{
    test_insert_and_search();
    test_duplicate_insert();
    test_empty_tree();
    test_split();
    test_delete_leaf_key();
    test_delete_nonexistent();
    test_delete_internal_key();
    test_traverse();
    test_stress();

    printf("All btree tests passed\n");
    return 0;
}