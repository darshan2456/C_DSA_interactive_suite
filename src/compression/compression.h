#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stdbool.h>

// RLE Compression Algorithms
int rle_encode(const char* input, char* output, int out_max);
int rle_decode(const char* input, char* output, int out_max);

// Huffman Coding Structures & Algorithms
typedef struct HuffmanNode
{
    char ch;
    int freq;
    struct HuffmanNode* left;
    struct HuffmanNode* right;
} HuffmanNode;

HuffmanNode* build_huffman_tree(const char* input);
void generate_huffman_codes(const HuffmanNode* root, char codes[256][256], char* current_code,
                            int depth);
int huffman_encode(const char* input, const char codes[256][256], char* output, int out_max);
int huffman_decode(const char* encoded_input, const HuffmanNode* root, char* output, int out_max);
void free_huffman_tree(HuffmanNode* root);

// Main Demo Menu
void compression_demo(void);

#endif // COMPRESSION_H
