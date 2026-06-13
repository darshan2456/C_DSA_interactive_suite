#include "history_logger.h"
#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



static void compute_lps_array(char* pattern, int m, int* lps)
{
    int length = 0;
    lps[0] = 0;
    int i = 1;

    while (i < m)
    {
        if (pattern[i] == pattern[length])
        {
            length++;
            lps[i] = length;
            i++;
        }
        else
        {
            if (length != 0)
            {
                length = lps[length - 1];
            }
            else
            {
                lps[i] = 0;
                i++;
            }
        }
    }

    printf("\n--- Prefix-Function (LPS Array) ---\n");
    printf("Pattern: ");
    for (int j = 0; j < m; j++)
    {
        printf("%c ", pattern[j]);
    }
    printf("\nLPS:     ");
    for (int j = 0; j < m; j++)
    {
        printf("%d ", lps[j]);
    }
    printf("\n-----------------------------------\n\n");
}

void kmp_search(char* text, char* pattern)
{
    int n = strlen(text);
    int m = strlen(pattern);
    int found = 0;

    if (m == 0) return;

    int* lps = (int*)malloc(m * sizeof(int));
    compute_lps_array(pattern, m, lps);

    int i = 0;
    int j = 0;
    while ((n - i) >= (m - j))
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }

        if (j == m)
        {
            printf("Pattern found at index %d\n", i - j);
            found++;
            j = lps[j - 1];
        }
        else if (i < n && pattern[j] != text[i])
        {
            if (j != 0)
            {
                j = lps[j - 1];
            }
            else
            {
                i = i + 1;
            }
        }
    }

    free(lps);

    if (found == 0)
    {
        printf("Pattern not found in the text.\n");
    }
    else
    {
        printf("Total occurrences found: %d\n", found);
    }
}

void kmp_demo(void)
{
    while (1)
    {
        char text[100];
        char pattern[100];

        printf("\nKnuth-Morris-Pratt (KMP) Demo\n");

        int status_T =
            safe_input_string(text, "Enter text (no spaces, max 99 chars), or 'X' to exit: ");
        if (status_T == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting demo...\n");
            return;
        }

        int status_P =
            safe_input_string(pattern, "Enter pattern (no spaces, max 99 chars), or 'X' to exit: ");
        if (status_P == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting demo...\n");
            return;
        }

        clock_t start_t = clock();
        kmp_search(text, pattern);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("Total CPU time taken: %f seconds\n", total_t);

        add_to_history("Knuth-Morris-Pratt (KMP)", strlen(text), total_t);
    }
}
