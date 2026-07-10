#ifndef COMPRESSION_H
#define COMPRESSION_H

#include <stdbool.h>

// RLE Compression Algorithms
int rle_encode(const char* input, char* output, int len, int out_max);
int rle_decode(const char* input, int len, char* output, int out_max);

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
void print_huffman_tree_visual(const HuffmanNode* root, const char* prefix, bool is_left);
void print_huffman_dictionary(const HuffmanNode* root, const char codes[256][256]);

// LZW Compression Algorithms
int lzw_encode(const char* input, int* output, int out_max);
int lzw_decode(const int* input, int in_len, char* output, int out_max);

// Burrows-Wheeler Transform (BWT)
int bwt_forward(const char* input, char* output, int* primary_index);
int bwt_inverse(const char* input, int primary_index, char* output);

// Move-To-Front (MTF)
int mtf_encode(const char* input, char* output, int len);
int mtf_decode(const char* input, char* output, int len);

// Main Demo Menu
void compression_demo(void);

#endif // COMPRESSION_H
