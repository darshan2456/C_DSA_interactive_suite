#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TrieNode* trie_create_node(void)
{
    TrieNode* node = malloc(sizeof(TrieNode));
    if (node == NULL)
        return NULL;
    memset(node, 0, sizeof(TrieNode));
    return node;
}

bool trie_insert(TrieNode* root, const char* word)
{
    if (root == NULL || word == NULL)
        return false;
    TrieNode* curr = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int idx = word[i] - 'a';
        if (idx < 0 || idx >= TRIE_ALPHA_SIZE)
            return false;
        if (curr->children[idx] == NULL)
        {
            curr->children[idx] = trie_create_node();
            if (curr->children[idx] == NULL)
                return false;
        }
        curr = curr->children[idx];
    }
    curr->is_end = 1;
    return true;
}

int trie_search(TrieNode* root, const char* word)
{
    if (root == NULL || word == NULL)
        return 0;
    TrieNode* curr = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int idx = word[i] - 'a';
        if (idx < 0 || idx >= TRIE_ALPHA_SIZE)
            return 0;
        if (curr->children[idx] == NULL)
            return 0;
        curr = curr->children[idx];
    }
    return curr->is_end;
}

int trie_starts_with_prefix(TrieNode* root, const char* prefix)
{
    if (root == NULL || prefix == NULL)
        return 0;
    TrieNode* curr = root;
    for (int i = 0; prefix[i] != '\0'; i++)
    {
        int idx = prefix[i] - 'a';
        if (idx < 0 || idx >= TRIE_ALPHA_SIZE)
            return 0;
        if (curr->children[idx] == NULL)
            return 0;
        curr = curr->children[idx];
    }
    return 1;
}

/* Returns 1 if the node has no children and can be freed */
static int trie_delete_helper(TrieNode* node, const char* word, int depth)
{
    if (node == NULL)
        return 0;
    if (word[depth] == '\0')
    {
        if (!node->is_end)
            return 0;
        node->is_end = 0;
        for (int i = 0; i < TRIE_ALPHA_SIZE; i++)
            if (node->children[i] != NULL)
                return 0;
        return 1;
    }
    int idx = word[depth] - 'a';
    if (idx < 0 || idx >= TRIE_ALPHA_SIZE)
        return 0;
    if (!trie_delete_helper(node->children[idx], word, depth + 1))
        return 0;
    free(node->children[idx]);
    node->children[idx] = NULL;
    if (node->is_end)
        return 0;
    for (int i = 0; i < TRIE_ALPHA_SIZE; i++)
        if (node->children[i] != NULL)
            return 0;
    return 1;
}

void trie_delete(TrieNode* root, const char* word)
{
    if (root == NULL || word == NULL)
        return;
    trie_delete_helper(root, word, 0);
}

void trie_free(TrieNode* node)
{
    if (node == NULL)
        return;
    for (int i = 0; i < TRIE_ALPHA_SIZE; i++)
        trie_free(node->children[i]);
    free(node);
}
