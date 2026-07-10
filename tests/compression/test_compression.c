#include "compression.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void test_rle_basic(void)
{
    char compressed[100];
    char decompressed[100];

    // Test 1: Empty input
    assert(rle_encode("", compressed, 0, sizeof(compressed)) == 0);
    assert(strcmp(compressed, "") == 0);
    assert(rle_decode("", 0, decompressed, sizeof(decompressed)) == 0);
    assert(strcmp(decompressed, "") == 0);

    // Test 2: Standard run-length encoding
    assert(rle_encode("aaaaabbbccccc", compressed, 13, sizeof(compressed)) > 0);
    assert(strcmp(compressed, "a5b3c5") == 0);

    // Test 3: Decompress standard output
    assert(rle_decode("a5b3c5", 6, decompressed, sizeof(decompressed)) > 0);
    assert(strcmp(decompressed, "aaaaabbbccccc") == 0);

    // Test 4: Single characters (no repetitions)
    assert(rle_encode("abc", compressed, 3, sizeof(compressed)) > 0);
    assert(strcmp(compressed, "a1b1c1") == 0);
    assert(rle_decode("a1b1c1", 6, decompressed, sizeof(decompressed)) > 0);
    assert(strcmp(decompressed, "abc") == 0);

    // Test 5: Multi-digit counts
    assert(rle_encode("aaaaaaaaaaaa", compressed, 12, sizeof(compressed)) > 0);
    assert(strcmp(compressed, "a12") == 0);
    assert(rle_decode("a12", 3, decompressed, sizeof(decompressed)) > 0);
    assert(strcmp(decompressed, "aaaaaaaaaaaa") == 0);

    // Test 6: Invalid decode formats
    assert(rle_decode("a", 1, decompressed, sizeof(decompressed)) == -1);
    assert(rle_decode("a-3", 3, decompressed, sizeof(decompressed)) == -1);

    // Test 7: Buffer limits
    assert(rle_encode("aaaaabbbccccc", compressed, 13, 5) == -1);
    assert(rle_decode("a5b3c5", 6, decompressed, 5) == -1);

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

static void test_lzw_basic(void)
{
    const char* input = "TOBEORNOTTOBEORTOBEORNOT";
    int compressed[512];
    int comp_len = lzw_encode(input, compressed, sizeof(compressed) / sizeof(compressed[0]));
    assert(comp_len > 0);

    char decompressed[256];
    int decomp_len = lzw_decode(compressed, comp_len, decompressed, sizeof(decompressed));
    assert(decomp_len > 0);
    assert(strcmp(decompressed, input) == 0);

    // Test dictionary reset on very long repetitive stream
    char* long_input = malloc(10000);
    for (int i = 0; i < 9990; i++)
    {
        long_input[i] = (char)('A' + (i % 26));
    }
    long_input[9990] = '\0';

    int* long_compressed = malloc(sizeof(int) * 10000);
    int long_comp_len = lzw_encode(long_input, long_compressed, 10000);
    assert(long_comp_len > 0);

    char* long_decompressed = malloc(10000);
    int long_decomp_len = lzw_decode(long_compressed, long_comp_len, long_decompressed, 10000);
    assert(long_decomp_len > 0);
    assert(strcmp(long_decompressed, long_input) == 0);

    free(long_input);
    free(long_compressed);
    free(long_decompressed);

    printf("LZW basic and dictionary reset tests passed\n");
}

static void test_bwt_mtf_basic(void)
{
    const char* input = "banana";
    char bwt_out[100];
    int primary_index = 0;
    int bwt_len = bwt_forward(input, bwt_out, &primary_index);
    assert(bwt_len > 0);
    assert(strcmp(bwt_out, "nnbaaa") == 0);
    assert(primary_index == 3);

    char bwt_dec[100];
    int bwt_dec_len = bwt_inverse(bwt_out, primary_index, bwt_dec);
    assert(bwt_dec_len > 0);
    assert(strcmp(bwt_dec, input) == 0);

    // Test MTF coding
    char mtf_out[100];
    int mtf_len = mtf_encode("nnbaaa", mtf_out, 6);
    assert(mtf_len == 6);

    char mtf_dec[100];
    int mtf_dec_len = mtf_decode(mtf_out, mtf_dec, mtf_len);
    assert(mtf_dec_len == 6);
    assert(memcmp(mtf_dec, "nnbaaa", 6) == 0);

    // Full pipeline test: input -> BWT -> MTF -> RLE -> decRLE -> decMTF -> invBWT -> input
    char rle_out[100];
    int rle_len = rle_encode(mtf_out, rle_out, mtf_len, sizeof(rle_out));
    assert(rle_len > 0);

    char rle_dec[100];
    int rle_dec_len = rle_decode(rle_out, rle_len, rle_dec, sizeof(rle_dec));
    assert(rle_dec_len == mtf_len);

    char final_mtf_dec[100];
    int final_mtf_dec_len = mtf_decode(rle_dec, final_mtf_dec, rle_dec_len);
    assert(final_mtf_dec_len == mtf_len);
    assert(memcmp(final_mtf_dec, bwt_out, bwt_len) == 0);

    char final_bwt_dec[100];
    int final_bwt_dec_len = bwt_inverse(final_mtf_dec, primary_index, final_bwt_dec);
    assert(final_bwt_dec_len == bwt_len);
    assert(strcmp(final_bwt_dec, input) == 0);

    printf("BWT and MTF basic and full pipeline tests passed\n");
}

int main(void)
{
    test_rle_basic();
    test_huffman_basic();
    test_huffman_visualizer();
    test_lzw_basic();
    test_bwt_mtf_basic();
    printf("All compression tests passed\n");
    return 0;
}
