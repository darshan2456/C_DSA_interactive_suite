#include "data_structures.h"
#include <assert.h>
#include <stdio.h>

void test_insert_and_search()
{
    TrieNode *root = trie_create_node();

    trie_insert(root, "apple");
    trie_insert(root, "app");

    assert(trie_search(root, "apple") == 1);
    assert(trie_search(root, "app") == 1);
    assert(trie_search(root, "ap") == 0);
    assert(trie_search(root, "appl") == 0);

    trie_free(root);
    printf("trie insert and search test passed\n");
}

void test_prefix()
{
    TrieNode *root = trie_create_node();

    trie_insert(root, "hello");

    assert(trie_starts_with_prefix(root, "hel") == 1);
    assert(trie_starts_with_prefix(root, "hello") == 1);
    assert(trie_starts_with_prefix(root, "xyz") == 0);
    assert(trie_starts_with_prefix(root, "") == 1);

    trie_free(root);
    printf("trie prefix test passed\n");
}

void test_delete()
{
    TrieNode *root = trie_create_node();

    trie_insert(root, "test");
    assert(trie_search(root, "test") == 1);

    trie_delete(root, "test");
    assert(trie_search(root, "test") == 0);

    trie_free(root);
    printf("trie delete test passed\n");
}

void test_delete_nonexistent()
{
    TrieNode *root = trie_create_node();

    trie_delete(root, "ghost");
    assert(trie_search(root, "ghost") == 0);

    trie_free(root);
    printf("trie delete nonexistent test passed\n");
}

void test_null_guard()
{
    trie_search(NULL, "hello");
    trie_insert(NULL, "hello");
    trie_delete(NULL, "hello");
    trie_starts_with_prefix(NULL, "he");

    TrieNode *root = trie_create_node();
    trie_search(root, NULL);
    trie_free(root);

    printf("trie null guard test passed\n");
}

void test_prefix_survives_delete()
{
    TrieNode *root = trie_create_node();

    trie_insert(root, "apple");
    trie_insert(root, "application");

    trie_delete(root, "apple");

    assert(trie_search(root, "apple") == 0);
    assert(trie_search(root, "application") == 1);
    assert(trie_starts_with_prefix(root, "app") == 1);

    trie_free(root);
    printf("trie prefix survives delete test passed\n");
}

int main()
{
    test_insert_and_search();
    test_prefix();
    test_delete();
    test_delete_nonexistent();
    test_null_guard();
    test_prefix_survives_delete();

    printf("All trie tests passed\n");
    return 0;
}