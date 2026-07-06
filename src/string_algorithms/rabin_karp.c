#include "../utils/config.h"
#include "clear_screen.h"
#include "cross_platform_timer.h"
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

    if (m == 0 || n == 0 || m > n)
        return;

    for (i = 0; i < m - 1; i++)
        h = (h * d) % q;

    for (i = 0; i < m; i++)
    {
        p = (d * p + (unsigned char)pattern[i]) % q;
        t = (d * t + (unsigned char)text[i]) % q;
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
        }

        if (i < n - m)
        {
            t = (d * (t - (unsigned char)text[i] * h) + (unsigned char)text[i + m]) % q;
            if (t < 0)
                t = (t + q);
        }
    }
    printf("Total Patterns Found: %d\n", found);
    printf("Total Collisions: %d\n", collisions);
}

void rabin_karp_visualization(char* text, char* pattern, int q)
{
    int m = strlen(pattern);
    int n = strlen(text);
    int i, j;
    int p = 0;
    int t = 0;
    int h = 1;
    int found = 0;
    int collisions = 0;
    int step = 1;

    if (m == 0 || n == 0 || m > n)
        return;
    for (i = 0; i < m - 1; i++)
        h = (h * d) % q;
    for (i = 0; i < m; i++)
    {
        p = (d * p + (unsigned char)pattern[i]) % q;
        t = (d * t + (unsigned char)text[i]) % q;
    }

    dynamic_sleep();

    for (i = 0; i <= n - m; i++)
    {
        if (!is_instant())
        {
            clear_screen();
        }
        printf("\nStep %d\n", step++);
        printf("Current Index : %d\n", i);
        printf("Text Window   : '");
        for (int k = 0; k < m; k++)
        {
            printf("%c", text[i + k]);
        }
        printf("'\n");
        printf("Pattern       : '%s'\n", pattern);

        printf("Window Hash   : %d\n", t);
        printf("Pattern Hash  : %d\n", p);
        printf("----------------------------------\n");

        if (p == t)
        {
            printf("Action        : Hashes MATCH! Checking characters...\n");
            dynamic_sleep();

            for (j = 0; j < m; j++)
            {
                if (text[i + j] != pattern[j])
                {
                    collisions++;
                    printf("Result        : 🔴 Mismatch at char '%c'. Spurious hit (collision)!\n",
                           text[i + j]);
                    break;
                }
            }

            if (j == m)
            {
                printf("Result        : 🟢 Pattern found at index %d!\n", i);
                found++;
            }
        }
        else
        {
            printf("Action        : Hashes mismatch. Rolling to next window...\n");
        }

        printf("----------------------------------\n");
        dynamic_sleep();
        if (i < n - m)
        {
            t = (d * (t - (unsigned char)text[i] * h) + (unsigned char)text[i + m]) % q;
            if (t < 0)
                t = (t + q);
        }
    }

    if (!is_instant())
    {
        clear_screen();
    }
    printf("\n==================================\n");
    printf("Rabin-Karp Search Complete\n");
    printf("==================================\n");
    if (found == 0)
    {
        printf("\nPattern not found in the text.\n");
    }
    else
    {
        printf("\nTotal occurrences found: %d\n", found);
    }
    printf("Total collisions: %d\n", collisions);
    printf("==================================\n");
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
        // 🚨 Point the demo to the visualization function 🚨
        rabin_karp_visualization(text, pattern, q);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("Total CPU time taken: %f seconds\n", total_t);
    }
}