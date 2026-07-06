#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

void naive_string_matching(char* text, char* pattern)
{
    int n = strlen(text);
    int m = strlen(pattern);
    int found = 0;

    for (int i = 0; i <= n - m; i++)
    {
        int j;
        for (j = 0; j < m; j++)
        {
            if (text[i + j] != pattern[j])
                break;
        }
        if (j == m)
        {
            printf("Pattern found at index %d\n", i);
            found++;
        }
    }

    if (found == 0)
    {
        printf("Pattern not found in the text.\n");
    }
    else
    {
        printf("Total occurrences found: %d\n", found);
    }
}

void naive_string_matching_demo(void)
{
    while (1)
    {
        char text[100];
        char pattern[100];

        printf("\nNaive String Matching Demo\n");

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
        naive_string_matching(text, pattern);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("Total CPU time taken: %f seconds\n", total_t);
    }
}
