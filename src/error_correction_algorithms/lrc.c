#include "error_correction_algorithms.h"
#include "safe_input.h"
#include <stdio.h>
#include <string.h>

/*
 * LRC (Longitudinal Redundancy Check)
 * ------------------------------------
 * Each bit of the LRC is the XOR (even parity) of the corresponding
 * bit column across all data words.  The result is appended to the
 * transmitted block so the receiver can verify integrity.
 */

#define LRC_MAX_ROWS 20
#define LRC_MAX_COLS 64

void lrc_demo(void)
{
    int rows;
    printf("\n=== LRC (Longitudinal Redundancy Check) ===\n");

    // Fix 1: Use safe_input_int() instead of scanf()
    int result = safe_input_int(&rows, "Enter number of data words (1-20): ", 1, LRC_MAX_ROWS);
    if (result == -111)
    {
        printf("Exiting LRC demo...\n");
        return;
    }
    if (result == 0)
    {
        printf("Invalid input. Returning to main menu.\n");
        return;
    }

    char data[LRC_MAX_ROWS][LRC_MAX_COLS + 1];
    int cols = -1;

    printf("Enter binary data words (same length, e.g. 10110011):\n");
    for (int i = 0; i < rows; i++)
    {
        // Fix 2: Use fgets() instead of scanf() for strings
        printf("  Word %d: ", i + 1);
        fgets(data[i], sizeof(data[i]), stdin);

        // Remove trailing newline; if absent, input exceeded buffer — flush stdin
        int len = strlen(data[i]);
        if (len > 0 && data[i][len - 1] == '\n')
        {
            data[i][len - 1] = '\0';
            len--;
        }
        else
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
        }

        /* validate: only '0' and '1' allowed */
        if (len == 0)
        {
            printf("Error: word cannot be empty.\n");
            return;
        }

        for (int j = 0; j < len; j++)
        {
            if (data[i][j] != '0' && data[i][j] != '1')
            {
                printf("Error: word must contain only 0s and 1s.\n");
                return;
            }
        }

        /* all words must have the same length */
        if (cols == -1)
        {
            cols = len;
        }
        else if (len != cols)
        {
            printf("Error: all data words must have the same length.\n");
            return;
        }
    }

    /* compute LRC: XOR each column across all rows */
    char lrc[LRC_MAX_COLS + 1];
    for (int j = 0; j < cols; j++)
    {
        int ones = 0;
        for (int i = 0; i < rows; i++)
        {
            if (data[i][j] == '1')
                ones++;
        }
        lrc[j] = (ones % 2 == 0) ? '0' : '1';
    }
    lrc[cols] = '\0';

    /* display results */
    printf("\nTransmitted Block:\n");
    printf("------------------\n");
    for (int i = 0; i < rows; i++)
        printf("  Data word %2d : %s\n", i + 1, data[i]);
    printf("  LRC          : %s\n", lrc);
    printf("------------------\n");
    printf("LRC = %s\n", lrc);

    printf("\nColumn-by-column parity breakdown:\n");
    for (int j = 0; j < cols; j++)
    {
        int ones = 0;
        for (int i = 0; i < rows; i++)
            if (data[i][j] == '1')
                ones++;
        printf("  Col %2d : %d one(s) -> parity bit = %c\n", j + 1, ones, lrc[j]);
    }
}