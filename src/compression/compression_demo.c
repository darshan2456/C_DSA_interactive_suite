#include "compression.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void run_rle_demo(void)
{
    display_header("Run-Length Encoding (RLE)");

    char input[256];
    int input_status = safe_input_string(
        input, "Enter a string to compress (e.g., aaaaabbbccccc), or 'X' to exit: ");
    if (input_status == INPUT_EXIT_SIGNAL)
    {
        return;
    }

    int original_len = strlen(input);
    if (original_len == 0)
    {
        printf("\nError: Empty string provided.\n");
        printf("\nPress [ENTER] to continue...");
        getchar();
        return;
    }

    char compressed[512];
    int comp_len = rle_encode(input, compressed, sizeof(compressed));
    if (comp_len < 0)
    {
        printf("\nError during RLE compression.\n");
        printf("\nPress [ENTER] to continue...");
        getchar();
        return;
    }

    char decompressed[256];
    int decomp_len = rle_decode(compressed, decompressed, sizeof(decompressed));

    // Display results
    printf("\n--- Real-Time RLE Compression Summary ---\n");
    printf("Original String  : \"%s\" (%d bytes)\n", input, original_len);
    printf("Compressed RLE   : \"%s\" (%d bytes)\n", compressed, comp_len);

    double ratio = (1.0 - (double)comp_len / original_len) * 100.0;
    printf("Compression Ratio: %.2f%%\n", ratio);

    if (decomp_len >= 0 && strcmp(input, decompressed) == 0)
    {
        printf("Round-trip Check : 🟢 PASSED (Successfully decompressed back to original)\n");
    }
    else
    {
        printf("Round-trip Check : 🔴 FAILED (Decompression mismatch)\n");
    }

    printf("\nCharacter Run Visualization:\n");
    int i = 0;
    while (i < original_len)
    {
        char ch = input[i];
        int count = 1;
        while (i + 1 < original_len && input[i + 1] == ch)
        {
            count++;
            i++;
        }
        i++;
        printf("  '%c' repeated %d time(s) -> \"%c%d\"\n", ch, count, ch, count);
    }

    printf("\nPress [ENTER] to continue...");
    getchar();
}

static void run_huffman_demo(void)
{
    display_header("Huffman Coding");

    char input[256];
    int input_status = safe_input_string(
        input, "Enter a string to compress (e.g., hello huffman), or 'X' to exit: ");
    if (input_status == INPUT_EXIT_SIGNAL)
    {
        return;
    }

    int original_len = strlen(input);
    if (original_len == 0)
    {
        printf("\nError: Empty string provided.\n");
        printf("\nPress [ENTER] to continue...");
        getchar();
        return;
    }

    HuffmanNode* root = build_huffman_tree(input);
    if (root == NULL)
    {
        printf("\nError: Failed to build Huffman tree.\n");
        printf("\nPress [ENTER] to continue...");
        getchar();
        return;
    }

    char codes[256][256];
    memset(codes, 0, sizeof(codes));
    char current_code[256];
    generate_huffman_codes(root, codes, current_code, 0);

    char compressed[1024];
    int comp_bits = huffman_encode(input, codes, compressed, sizeof(compressed));
    if (comp_bits < 0)
    {
        printf("\nError during Huffman encoding.\n");
        free_huffman_tree(root);
        printf("\nPress [ENTER] to continue...");
        getchar();
        return;
    }

    char decompressed[256];
    int decomp_len = huffman_decode(compressed, root, decompressed, sizeof(decompressed));

    // Display results
    printf("\n--- Huffman Tree Visualization ---\n");
    print_huffman_tree_visual(root, "", false);

    print_huffman_dictionary(root, codes);

    printf("\n--- Huffman Compression Summary ---\n");
    printf("Original String   : \"%s\" (%d bytes / %d bits)\n", input, original_len,
           original_len * 8);
    printf("Compressed Stream : \"%s\" (%d bits / %.2f bytes)\n", compressed, comp_bits,
           (double)comp_bits / 8.0);

    double ratio = (1.0 - ((double)comp_bits / 8.0) / original_len) * 100.0;
    printf("Compression Ratio : %.2f%%\n", ratio);

    if (decomp_len >= 0 && strcmp(input, decompressed) == 0)
    {
        printf("Round-trip Check  : 🟢 PASSED (Successfully decompressed back to original)\n");
    }
    else
    {
        printf("Round-trip Check  : 🔴 FAILED (Decompression mismatch)\n");
    }

    free_huffman_tree(root);

    printf("\nPress [ENTER] to continue...");
    getchar();
}

void compression_demo(void)
{
    while (1)
    {
        display_header("String Compression & Encoding Suite");

        int choice;
        int status = safe_input_int(&choice,
                                    "\nSelect Compression Algorithm:\n"
                                    "1. Run-Length Encoding (RLE)\n"
                                    "2. Huffman Coding\n"
                                    "Enter choice (1 to 2), or '-1' to exit: ",
                                    1, 2);

        if (status == INPUT_EXIT_SIGNAL)
        {
            return;
        }
        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                run_rle_demo();
                break;
            case 2:
                run_huffman_demo();
                break;
            default:
                break;
        }
    }
}
