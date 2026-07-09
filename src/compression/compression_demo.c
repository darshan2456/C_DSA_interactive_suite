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

void compression_demo(void)
{
    while (1)
    {
        display_header("String Compression & Encoding Suite");

        int choice;
        int status = safe_input_int(&choice,
                                    "\nSelect Compression Algorithm:\n"
                                    "1. Run-Length Encoding (RLE)\n"
                                    "Enter choice (1), or '-1' to exit: ",
                                    1, 1);

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
            default:
                break;
        }
    }
}
