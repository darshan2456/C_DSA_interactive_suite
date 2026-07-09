#include "compression.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    HuffmanNode* nodes[256];
    int size;
} MinHeap;

static void heap_push(MinHeap* heap, HuffmanNode* node)
{
    int i = heap->size++;
    while (i > 0)
    {
        int p = (i - 1) / 2;
        if (heap->nodes[p]->freq <= node->freq)
        {
            break;
        }
        heap->nodes[i] = heap->nodes[p];
        i = p;
    }
    heap->nodes[i] = node;
}

static HuffmanNode* heap_pop(MinHeap* heap)
{
    if (heap->size <= 0)
    {
        return NULL;
    }
    HuffmanNode* min_node = heap->nodes[0];
    heap->size--;
    if (heap->size > 0)
    {
        HuffmanNode* last = heap->nodes[heap->size];
        int i = 0;
        while (i * 2 + 1 < heap->size)
        {
            int left = i * 2 + 1;
            int right = left + 1;
            int child = left;
            if (right < heap->size && heap->nodes[right]->freq < heap->nodes[left]->freq)
            {
                child = right;
            }
            if (last->freq <= heap->nodes[child]->freq)
            {
                break;
            }
            heap->nodes[i] = heap->nodes[child];
            i = child;
        }
        heap->nodes[i] = last;
    }
    return min_node;
}

HuffmanNode* build_huffman_tree(const char* input)
{
    if (input == NULL || strlen(input) == 0)
    {
        return NULL;
    }

    int freq[256] = {0};
    int len = strlen(input);
    for (int i = 0; i < len; i++)
    {
        freq[(unsigned char)input[i]]++;
    }

    MinHeap heap;
    heap.size = 0;

    for (int i = 0; i < 256; i++)
    {
        if (freq[i] > 0)
        {
            HuffmanNode* node = malloc(sizeof(HuffmanNode));
            node->ch = (char)i;
            node->freq = freq[i];
            node->left = NULL;
            node->right = NULL;
            heap_push(&heap, node);
        }
    }

    if (heap.size == 0)
    {
        return NULL;
    }

    // Handle single unique character string
    if (heap.size == 1)
    {
        HuffmanNode* leaf = heap_pop(&heap);
        HuffmanNode* parent = malloc(sizeof(HuffmanNode));
        parent->ch = '\0';
        parent->freq = leaf->freq;
        parent->left = leaf;
        parent->right = NULL;
        return parent;
    }

    while (heap.size > 1)
    {
        HuffmanNode* left = heap_pop(&heap);
        HuffmanNode* right = heap_pop(&heap);

        HuffmanNode* parent = malloc(sizeof(HuffmanNode));
        parent->ch = '\0';
        parent->freq = left->freq + right->freq;
        parent->left = left;
        parent->right = right;

        heap_push(&heap, parent);
    }

    return heap_pop(&heap);
}

void generate_huffman_codes(const HuffmanNode* root, char codes[256][256], char* current_code,
                            int depth)
{
    if (root == NULL)
    {
        return;
    }

    // Leaf node
    if (root->left == NULL && root->right == NULL)
    {
        current_code[depth] = '\0';
        strcpy(codes[(unsigned char)root->ch], current_code);
        return;
    }

    if (root->left != NULL)
    {
        current_code[depth] = '0';
        generate_huffman_codes(root->left, codes, current_code, depth + 1);
    }

    if (root->right != NULL)
    {
        current_code[depth] = '1';
        generate_huffman_codes(root->right, codes, current_code, depth + 1);
    }
}

int huffman_encode(const char* input, const char codes[256][256], char* output, int out_max)
{
    if (input == NULL || output == NULL || out_max <= 0)
    {
        return -1;
    }

    int out_idx = 0;
    int len = strlen(input);

    for (int i = 0; i < len; i++)
    {
        const char* code = codes[(unsigned char)input[i]];
        int code_len = strlen(code);

        if (out_idx + code_len >= out_max)
        {
            return -1; // Buffer overflow
        }

        strcpy(&output[out_idx], code);
        out_idx += code_len;
    }

    output[out_idx] = '\0';
    return out_idx;
}

int huffman_decode(const char* encoded_input, const HuffmanNode* root, char* output, int out_max)
{
    if (encoded_input == NULL || root == NULL || output == NULL || out_max <= 0)
    {
        return -1;
    }

    int in_len = strlen(encoded_input);
    int out_idx = 0;
    const HuffmanNode* curr = root;

    for (int i = 0; i < in_len; i++)
    {
        char bit = encoded_input[i];
        if (bit == '0')
        {
            curr = curr->left;
        }
        else if (bit == '1')
        {
            curr = curr->right;
        }
        else
        {
            return -1; // Invalid bit
        }

        if (curr == NULL)
        {
            return -1; // Tree traversal failure
        }

        // Leaf node
        if (curr->left == NULL && curr->right == NULL)
        {
            if (out_idx >= out_max - 1)
            {
                return -1; // Buffer overflow
            }
            output[out_idx++] = curr->ch;
            curr = root;
        }
    }

    output[out_idx] = '\0';
    return out_idx;
}

void free_huffman_tree(HuffmanNode* root)
{
    if (root == NULL)
    {
        return;
    }
    free_huffman_tree(root->left);
    free_huffman_tree(root->right);
    free(root);
}
