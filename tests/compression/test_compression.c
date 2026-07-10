#include "compression.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static void test_rle_basic(void)
{
    char compressed[100];
    char decompressed[100];

    // Test 1: Empty input
    assert(rle_encode("", compressed, sizeof(compressed)) == 0);
    assert(strcmp(compressed, "") == 0);
    assert(rle_decode("", decompressed, sizeof(decompressed)) == 0);
    assert(strcmp(decompressed, "") == 0);

    // Test 2: Standard run-length encoding
    assert(rle_encode("aaaaabbbccccc", compressed, sizeof(compressed)) > 0);
    assert(strcmp(compressed, "a5b3c5") == 0);

    // Test 3: Decompress standard output
    assert(rle_decode("a5b3c5", decompressed, sizeof(decompressed)) > 0);
    assert(strcmp(decompressed, "aaaaabbbccccc") == 0);

    // Test 4: Single characters (no repetitions)
    assert(rle_encode("abc", compressed, sizeof(compressed)) > 0);
    assert(strcmp(compressed, "a1b1c1") == 0);
    assert(rle_decode("a1b1c1", decompressed, sizeof(decompressed)) > 0);
    assert(strcmp(decompressed, "abc") == 0);

    // Test 5: Multi-digit counts
    assert(rle_encode("aaaaaaaaaaaa", compressed, sizeof(compressed)) > 0);
    assert(strcmp(compressed, "a12") == 0);
    assert(rle_decode("a12", decompressed, sizeof(decompressed)) > 0);
    assert(strcmp(decompressed, "aaaaaaaaaaaa") == 0);

    // Test 6: Invalid decode formats
    assert(rle_decode("a", decompressed, sizeof(decompressed)) == -1);
    assert(rle_decode("a-3", decompressed, sizeof(decompressed)) == -1);

    // Test 7: Buffer limits
    assert(rle_encode("aaaaabbbccccc", compressed, 5) == -1);
    assert(rle_decode("a5b3c5", decompressed, 5) == -1);

    printf("RLE basic tests passed\n");
}

static void test_huffman_basic(void)
{
    const char* input = "hello huffman";
    HuffmanNode* root = build_huffman_tree(input);
    assert(root != NULL);

    char codes[256][256];
    memset(codes, 0, sizeof(codes));
    char current_code[256];
    generate_huffman_codes(root, codes, current_code, 0);

    // Verify code generation for present characters
    assert(strlen(codes[(unsigned char)'h']) > 0);
    assert(strlen(codes[(unsigned char)'e']) > 0);
    assert(strlen(codes[(unsigned char)'l']) > 0);
    assert(strlen(codes[(unsigned char)'o']) > 0);
    assert(strlen(codes[(unsigned char)' ']) > 0);

    // Encode
    char encoded[1024];
    int enc_len = huffman_encode(input, codes, encoded, sizeof(encoded));
    assert(enc_len > 0);

    // Decode
    char decoded[256];
    int dec_len = huffman_decode(encoded, root, decoded, sizeof(decoded));
    assert(dec_len > 0);
    assert(strcmp(decoded, input) == 0);

    // Test single unique character input
    HuffmanNode* single_root = build_huffman_tree("aaaa");
    assert(single_root != NULL);
    char single_codes[256][256];
    memset(single_codes, 0, sizeof(single_codes));
    generate_huffman_codes(single_root, single_codes, current_code, 0);
    assert(strlen(single_codes[(unsigned char)'a']) > 0);

    char single_encoded[100];
    int single_enc_len =
        huffman_encode("aaaa", single_codes, single_encoded, sizeof(single_encoded));
    assert(single_enc_len > 0);

    char single_decoded[100];
    int single_dec_len =
        huffman_decode(single_encoded, single_root, single_decoded, sizeof(single_decoded));
    assert(single_dec_len > 0);
    assert(strcmp(single_decoded, "aaaa") == 0);

    free_huffman_tree(root);
    free_huffman_tree(single_root);
    printf("Huffman basic tests passed\n");
}

static void test_huffman_visualizer(void)
{
    const char* input = "hello huffman";
    HuffmanNode* root = build_huffman_tree(input);
    assert(root != NULL);

    char codes[256][256];
    memset(codes, 0, sizeof(codes));
    char current_code[256];
    generate_huffman_codes(root, codes, current_code, 0);

    print_huffman_tree_visual(root, "", false);
    print_huffman_dictionary(root, codes);

    print_huffman_tree_visual(NULL, "", false);
    print_huffman_dictionary(NULL, codes);

    free_huffman_tree(root);
    printf("Huffman visualizer tests passed\n");
}

int main(void)
{
    test_rle_basic();
    test_huffman_basic();
    test_huffman_visualizer();
    printf("All compression tests passed\n");
    return 0;
}
