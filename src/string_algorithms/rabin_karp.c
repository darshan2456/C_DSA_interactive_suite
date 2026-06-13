#include "history_logger.h"
#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define d 256



void rabin_karp_search(char* text, char* pattern, int q)
{
    int m = strlen(pattern);
    int n = strlen(text);
    int i, j;
    int p = 0; // hash value for pattern
    int t = 0; // hash value for text
    int h = 1;
    int found = 0;
    int collisions = 0;

    if (m == 0) return;

    for (i = 0; i < m - 1; i++)
        h = (h * d) % q;

    for (i = 0; i < m; i++)
    {
        p = (d * p + pattern[i]) % q;
        t = (d * t + text[i]) % q;
    }

    for (i = 0; i <= n - m; i++)
    {
        if (p == t)
        {
            for (j = 0; j < m; j++)
            {
                if (text[i + j] != pattern[j])
                {
                    collisions++;
                    break;
                }
            }

            if (j == m)
            {
                printf("Pattern found at index %d\n", i);
                found++;
            }
            else
            {
                printf("Spurious hit (collision) at index %d\n", i);
            }
        }

        if (i < n - m)
        {
            t = (d * (t - text[i] * h) + text[i + m]) % q;
            if (t < 0)
                t = (t + q);
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
    printf("Total spurious hits (collisions): %d\n", collisions);
}

void rabin_karp_demo(void)
{
    while (1)
    {
        char text[100];
        char pattern[100];
        int q = 101; // A prime number

        printf("\nRabin-Karp Algorithm Demo\n");

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
        rabin_karp_search(text, pattern, q);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("Total CPU time taken: %f seconds\n", total_t);

        add_to_history("Rabin-Karp Algorithm", strlen(text), total_t);
    }
}
