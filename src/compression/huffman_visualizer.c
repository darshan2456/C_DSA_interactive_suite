#include "compression.h"
#include <stdio.h>
#include <string.h>

void print_huffman_tree_visual(const HuffmanNode* root, const char* prefix, bool is_left)
{
    if (root == NULL)
    {
        return;
    }

    printf("%s", prefix);
    printf(is_left ? "├── " : "└── ");

    if (root->left == NULL && root->right == NULL)
    {
        // Leaf node
        if (root->ch == '\n')
        {
            printf("[Freq:%d] '\\n'\n", root->freq);
        }
        else if (root->ch == '\t')
        {
            printf("[Freq:%d] '\\t'\n", root->freq);
        }
        else if (root->ch == ' ')
        {
            printf("[Freq:%d] 'Space'\n", root->freq);
        }
        else
        {
            printf("[Freq:%d] '%c'\n", root->freq, root->ch);
        }
    }
    else
    {
        printf("[Freq:%d] (Internal)\n", root->freq);
    }

    char new_prefix[512];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_left ? "│   " : "    ");

    if (root->left != NULL)
    {
        print_huffman_tree_visual(root->left, new_prefix, true);
    }
    if (root->right != NULL)
    {
        print_huffman_tree_visual(root->right, new_prefix, false);
    }
}

static void collect_leaf_nodes(const HuffmanNode* root, const char codes[256][256])
{
    if (root == NULL)
    {
        return;
    }

    if (root->left == NULL && root->right == NULL)
    {
        char ch_display[16];
        if (root->ch == '\n')
        {
            strcpy(ch_display, "'\\n'");
        }
        else if (root->ch == '\t')
        {
            strcpy(ch_display, "'\\t'");
        }
        else if (root->ch == ' ')
        {
            strcpy(ch_display, "'Space'");
        }
        else
        {
            snprintf(ch_display, sizeof(ch_display), "'%c'", root->ch);
        }

        printf("│ %-9s │ %-9d │ %-15s │\n", ch_display, root->freq, codes[(unsigned char)root->ch]);
        return;
    }

    collect_leaf_nodes(root->left, codes);
    collect_leaf_nodes(root->right, codes);
}

void print_huffman_dictionary(const HuffmanNode* root, const char codes[256][256])
{
    if (root == NULL)
    {
        return;
    }

    printf("\nGenerated Bit-Code Dictionary Table:\n");
    printf("┌───────────┬───────────┬─────────────────┐\n");
    printf("│ Character │ Frequency │ Bit Code        │\n");
    printf("├───────────┼───────────┼─────────────────┤\n");
    collect_leaf_nodes(root, codes);
    printf("└───────────┴───────────┴─────────────────┘\n");
}
