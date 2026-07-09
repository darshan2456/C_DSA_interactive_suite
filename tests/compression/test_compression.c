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

int main(void)
{
    test_rle_basic();
    printf("All compression tests passed\n");
    return 0;
}
