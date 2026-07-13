#include "safe_input.h"
#include "trees.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void trie_demo(void)
{
    TrieNode* root = trie_create_node();
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
                                    "\nenter choice (\'-1\' to exit, or \'help\') : ",
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
            status = safe_input_string(word, "enter word (lowercase letters only): ");
            if (status == INPUT_EXIT_SIGNAL || status == 0)
                continue;
            if (trie_insert(root, word))
                printf("inserted: %s\n", word);
            else
                printf("\nMemory allocation failed or invalid input during insertion.\n");
        }
        else if (choice == 2)
        {
            status = safe_input_string(word, "enter word to search: ");
            if (status == INPUT_EXIT_SIGNAL || status == 0)
                continue;
            if (trie_search(root, word))
                printf("'%s' found in trie.\n", word);
            else
                printf("'%s' not found in trie.\n", word);
        }
        else if (choice == 3)
        {
            status = safe_input_string(word, "enter prefix: ");
            if (status == INPUT_EXIT_SIGNAL || status == 0)
                continue;
            if (trie_starts_with_prefix(root, word))
                printf("words with prefix '%s' exist.\n", word);
            else
                printf("no words with prefix '%s'.\n", word);
        }
        else if (choice == 4)
        {
            status = safe_input_string(word, "enter word to delete: ");
            if (status == INPUT_EXIT_SIGNAL || status == 0)
                continue;
            trie_delete(root, word);
            printf("delete attempted for: %s\n", word);
        }
    }
}