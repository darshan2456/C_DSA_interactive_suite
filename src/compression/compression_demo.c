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
    int comp_len = rle_encode(input, compressed, original_len, sizeof(compressed));
    if (comp_len < 0)
    {
        printf("\nError during RLE compression.\n");
        printf("\nPress [ENTER] to continue...");
        getchar();
        return;
    }

    char decompressed[256];
    int decomp_len = rle_decode(compressed, comp_len, decompressed, sizeof(decompressed));

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

static void run_lzw_demo(void)
{
    display_header("Lempel-Ziv-Welch (LZW) Coding");

    char input[256];
    int input_status = safe_input_string(
        input, "Enter a string to compress (e.g., TOBEORNOTTOBEORTOBEORNOT), or 'X' to exit: ");
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

    int compressed[512];
    int comp_len = lzw_encode(input, compressed, sizeof(compressed) / sizeof(compressed[0]));
    if (comp_len < 0)
    {
        printf("\nError during LZW compression.\n");
        printf("\nPress [ENTER] to continue...");
        getchar();
        return;
    }

    char decompressed[256];
    int decomp_len = lzw_decode(compressed, comp_len, decompressed, sizeof(decompressed));

    // Display results
    printf("\n--- LZW Compression Summary ---\n");
    printf("Original String   : \"%s\" (%d bytes / %d bits)\n", input, original_len,
           original_len * 8);
    printf("Compressed Codes  : [ ");
    for (int i = 0; i < comp_len; i++)
    {
        printf("%d ", compressed[i]);
    }
    printf("] (%d codes / %d bits at 12-bits per code)\n", comp_len, comp_len * 12);

    double ratio = (1.0 - ((double)(comp_len * 12) / 8.0) / original_len) * 100.0;
    printf("Compression Ratio : %.2f%%\n", ratio);

    if (decomp_len >= 0 && strcmp(input, decompressed) == 0)
    {
        printf("Round-trip Check  : 🟢 PASSED (Successfully decompressed back to original)\n");
    }
    else
    {
        printf("Round-trip Check  : 🔴 FAILED (Decompression mismatch)\n");
    }

    printf("\nPress [ENTER] to continue...");
    getchar();
}

static void run_bwt_mtf_demo(void)
{
    display_header("Burrows-Wheeler (BWT) & Move-To-Front (MTF)");

    char input[256];
    int input_status =
        safe_input_string(input, "Enter a string to transform (e.g., banana), or 'X' to exit: ");
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

    char bwt_out[256];
    int primary_index = 0;
    int bwt_len = bwt_forward(input, bwt_out, &primary_index);
    if (bwt_len < 0)
    {
        printf("\nError during BWT transform.\n");
        printf("\nPress [ENTER] to continue...");
        getchar();
        return;
    }

    char mtf_out[256];
    int mtf_len = mtf_encode(bwt_out, mtf_out, bwt_len);

    char rle_out[512];
    int rle_len = rle_encode(mtf_out, rle_out, mtf_len, sizeof(rle_out));

    char rle_dec[256];
    int rle_dec_len = rle_decode(rle_out, rle_len, rle_dec, sizeof(rle_dec));

    char mtf_dec[256];
    mtf_decode(rle_dec, mtf_dec, rle_dec_len);

    char bwt_dec[256];
    int bwt_dec_len = bwt_inverse(mtf_dec, primary_index, bwt_dec);

    // Display results
    printf("\n--- BWT & MTF Transformation Steps ---\n");
    printf("Original String    : \"%s\" (%d bytes)\n", input, original_len);
    printf("BWT Output (Last)  : \"%s\" (Primary Index: %d)\n", bwt_out, primary_index);
    printf("MTF Code Indexes   : [ ");
    for (int i = 0; i < mtf_len; i++)
    {
        printf("%d ", (unsigned char)mtf_out[i]);
    }
    printf("]\n");
    printf("RLE on MTF Output  : \"%s\" (%d bytes)\n", rle_out, rle_len);

    double ratio = (1.0 - (double)rle_len / original_len) * 100.0;
    printf("Compression Ratio  : %.2f%%\n", ratio);

    if (bwt_dec_len >= 0 && strcmp(input, bwt_dec) == 0)
    {
        printf("Round-trip Check   : 🟢 PASSED (Successfully reconstructed back to original)\n");
    }
    else
    {
        printf("Round-trip Check   : 🔴 FAILED (Reconstruction mismatch)\n");
    }

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
                                    "3. Lempel-Ziv-Welch (LZW)\n"
                                    "4. Burrows-Wheeler & Move-to-Front\n"
                                    "\nenter choice (\'-1\' to exit, or \'help\') : ",
                                    1, 4);

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
            case 3:
                run_lzw_demo();
                break;
            case 4:
                run_bwt_mtf_demo();
                break;
            default:
                break;
        }
    }
}
