#include "data_structures.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TrieNode *trie_create_node(void)
{
    TrieNode *node = malloc(sizeof(TrieNode));
    if (node == NULL)
        return NULL;
    memset(node, 0, sizeof(TrieNode));
    return node;
}

void trie_insert(TrieNode *root, const char *word)
{
    if (root == NULL || word == NULL)
        return;
    TrieNode *curr = root;
    for (int i = 0; word[i] != '\0'; i++)
    {
        int idx = word[i] - 'a';
        if (idx < 0 || idx >= TRIE_ALPHA_SIZE)
            return;
        if (curr->children[idx] == NULL)
            curr->children[idx] = trie_create_node();
        curr = curr->children[idx];
    }
    curr->is_end = 1;
}

int trie_search(TrieNode *root, const char *word)
{
    if (root == NULL || word == NULL)
        return 0;
    TrieNode *curr = root;
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

int trie_starts_with_prefix(TrieNode *root, const char *prefix)
{
    if (root == NULL || prefix == NULL)
        return 0;
    TrieNode *curr = root;
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
static int trie_delete_helper(TrieNode *node, const char *word, int depth)
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

void trie_delete(TrieNode *root, const char *word)
{
    if (root == NULL || word == NULL)
        return;
    trie_delete_helper(root, word, 0);
}

void trie_free(TrieNode *node)
{
    if (node == NULL)
        return;
    for (int i = 0; i < TRIE_ALPHA_SIZE; i++)
        trie_free(node->children[i]);
    free(node);
}

void trie_demo(void)
{
    TrieNode *root = trie_create_node();
    if (root == NULL)
    {
        printf("memory allocation failed\n");
        return;
    }

    while (1)
    {
        int choice;
        int status = safe_input_int(&choice,
                                    "\nenter 1 to insert word"
                                    "\nenter 2 to search word"
                                    "\nenter 3 to check prefix"
                                    "\nenter 4 to delete word"
                                    "\nenter choice : ",
                                    1, 4);

        if (status == INPUT_EXIT_SIGNAL)
        {
            trie_free(root);
            printf("\nexiting trie demo....\n");
            return;
        }
        if (status == 0)
            continue;

        char word[256];

        if (choice == 1)
        {
            printf("enter word (lowercase letters only): ");
            if (scanf("%255s", word) != 1)
                continue;
            trie_insert(root, word);
            printf("inserted: %s\n", word);
        }
        else if (choice == 2)
        {
            printf("enter word to search: ");
            if (scanf("%255s", word) != 1)
                continue;
            if (trie_search(root, word))
                printf("'%s' found in trie.\n", word);
            else
                printf("'%s' not found in trie.\n", word);
        }
        else if (choice == 3)
        {
            printf("enter prefix: ");
            if (scanf("%255s", word) != 1)
                continue;
            if (trie_starts_with_prefix(root, word))
                printf("words with prefix '%s' exist.\n", word);
            else
                printf("no words with prefix '%s'.\n", word);
        }
        else if (choice == 4)
        {
            printf("enter word to delete: ");
            if (scanf("%255s", word) != 1)
                continue;
            trie_delete(root, word);
            printf("delete attempted for: %s\n", word);
        }
    }

}