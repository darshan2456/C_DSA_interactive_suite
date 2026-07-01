#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
#include "history_logger.h"
#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void kmp_search(char* text, char* pattern)
{
    if (!text || !pattern)
        return;

    int n = strlen(text);
    int m = strlen(pattern);

    if (m == 0 || n == 0 || m > n)
        return;

    int* lps = (int*)malloc(m * sizeof(int));
    if (!lps)
        return;

    // Compute LPS
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

    // Search
    i = 0;
    int j = 0;
    while (i < n)
    {
        if (pattern[j] == text[i])
        {
            j++;
            i++;
        }

        if (j == m)
        {
            // 🚨 CRITICAL: The CI pipeline captures this exact string to count matches! 🚨
            printf("Pattern found at index %d\n", i - j);
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
                i++;
            }
        }
    }

    free(lps);
}

static void compute_lps_array_visual(char* pattern, int m, int* lps)
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

void kmp_visualization(char* text, char* pattern)
{
    int n = strlen(text);
    int m = strlen(pattern);
    int found = 0;
    int step = 1;
    int matches[100];
    int match_count = 0;

    if (m == 0 || n == 0)
        return;

    int* lps = (int*)malloc(m * sizeof(int));
    compute_lps_array_visual(pattern, m, lps);
    dynamic_sleep();

    int i = 0;
    int j = 0;
    while (i < n)
    {
        if (!is_instant())
        {
            clear_screen();
        }
        printf("\nStep %d\n", step++);
        printf("Text Index    : %d\n", i);
        printf("Pattern Index : %d\n", j);
        if (i < n)
        {
            printf("Text Char     : %c\n", text[i]);
        }
        if (j < m)
        {
            printf("Pattern Char  : %c\n", pattern[j]);
        }
        if (j < m && pattern[j] == text[i])
        {
            printf("Action        : Characters matched\n");
            j++;
            i++;
            printf("Next State    : i=%d, j=%d\n", i, j);
            printf("----------------------------------\n");
            dynamic_sleep();
        }

        if (j == m)
        {
            if (match_count < 100)
            {
                matches[match_count++] = i - j;
            }
            printf("Pattern found at index %d\n", i - j);
            printf("LPS Jump      : j = lps[%d] = %d\n", j - 1, lps[j - 1]);
            found++;
            j = lps[j - 1];
            dynamic_sleep();
        }
        else if (i < n && pattern[j] != text[i])
        {
            printf("Action        : Mismatch detected\n");
            if (j != 0)
            {
                printf("LPS Jump      : j = lps[%d] = %d\n", j - 1, lps[j - 1]);
                j = lps[j - 1];
            }
            else
            {
                printf("Action        : Move text pointer forward\n");
                i++;
            }

            printf("----------------------------------\n");
            dynamic_sleep();
        }
    }

    free(lps);
    if (!is_instant())
    {
        clear_screen();
    }
    printf("\n==================================\n");
    printf("KMP Search Complete\n");
    printf("==================================\n");
    if (found == 0)
    {
        printf("\nPattern not found in the text.\n");
    }
    else
    {
        printf("\nMatches Found:\n\n");

        for (int k = 0; k < match_count; k++)
        {
            printf("Pattern found at index %d\n", matches[k]);
        }

        printf("\nTotal occurrences found: %d\n", found);
    }

    printf("==================================\n");
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
        kmp_visualization(text, pattern);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("Total CPU time taken: %f seconds\n", total_t);

        add_to_history("Knuth-Morris-Pratt (KMP)", strlen(text), total_t);
    }
}