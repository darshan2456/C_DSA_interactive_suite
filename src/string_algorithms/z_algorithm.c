#include "../utils/config.h"
#include "clear_screen.h"
#include "history_logger.h"
#include "safe_input.h"
#include "string_algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void z_algorithm_search(char* text, char* pattern)
{
    int pat_len = strlen(pattern);
    int txt_len = strlen(text);

    if (pat_len == 0 || txt_len < pat_len)
    {
        printf("Pattern not found in the text.\n");
        return;
    }

    int concat_len = pat_len + 1 + txt_len;
    char* concat = (char*)malloc((concat_len + 1) * sizeof(char));
    int* Z = (int*)malloc(concat_len * sizeof(int));
    if (!concat || !Z)
    {
        printf("Memory allocation failed.\n");
        free(concat);
        free(Z);
        return;
    }

    strcpy(concat, pattern);
    concat[pat_len] = '$';
    strcpy(concat + pat_len + 1, text);

    // Z-array construction
    int L = 0, R = 0;
    Z[0] = 0;
    int found = 0;

    for (int i = 1; i < concat_len; i++)
    {
        if (i > R)
        {
            L = R = i;
            while (R < concat_len && concat[R - L] == concat[R])
                R++;
            Z[i] = R - L;
            R--;
        }
        else
        {
            int k = i - L;
            if (Z[k] < R - i + 1)
            {
                Z[i] = Z[k];
            }
            else
            {
                L = i;
                while (R < concat_len && concat[R - L] == concat[R])
                    R++;
                Z[i] = R - L;
                R--;
            }
        }

        if (Z[i] == pat_len)
        {
            printf("Pattern found at index %d\n", i - pat_len - 1);
            found++;
        }
    }

    free(concat);
    free(Z);

    if (found == 0)
    {
        printf("Pattern not found in the text.\n");
    }
    else
    {
        printf("Total occurrences found: %d\n", found);
    }
}

void z_algorithm_visualization(char* text, char* pattern)
{
    int pat_len = strlen(pattern);
    int txt_len = strlen(text);

    if (pat_len == 0 || txt_len < pat_len)
    {
        printf("Pattern not found in the text.\n");
        return;
    }

    int concat_len = pat_len + 1 + txt_len;
    char* concat = (char*)malloc((concat_len + 1) * sizeof(char));
    int* Z = (int*)malloc(concat_len * sizeof(int));
    if (!concat || !Z)
    {
        printf("Memory allocation failed.\n");
        free(concat);
        free(Z);
        return;
    }

    strcpy(concat, pattern);
    concat[pat_len] = '$';
    strcpy(concat + pat_len + 1, text);

    int L = 0, R = 0;
    Z[0] = 0;
    int found = 0;
    int step = 1;

    for (int i = 1; i < concat_len; i++)
    {
        if (!is_instant())
        {
            clear_screen();
        }

        printf("\nStep %d\n", step++);
        printf("Concat String: %s\n", concat);
        printf("Pointers     : ");
        for (int p = 0; p < concat_len; p++)
        {
            if (p == i)
                printf("i");
            else if (p == L && L != 0)
                printf("L");
            else if (p == R && R != 0)
                printf("R");
            else
                printf(" ");
        }
        printf("\n");

        if (i > R)
        {
            printf("Case         : i > R. Naively comparing suffix starting at index %d\n", i);
            L = R = i;
            while (R < concat_len && concat[R - L] == concat[R])
                R++;
            Z[i] = R - L;
            R--;
            printf("Z[%d] computed naively as %d. New Z-box bounds: [L=%d, R=%d]\n", i, Z[i], L, R);
        }
        else
        {
            int k = i - L;
            printf("Case         : i <= R (inside Z-box [L=%d, R=%d]). Checking Z[%d] = %d\n", L, R, k, Z[k]);
            if (Z[k] < R - i + 1)
            {
                Z[i] = Z[k];
                printf("Z[%d] copied directly from Z[%d] as %d (since prefix stays inside Z-box)\n", i, k, Z[i]);
            }
            else
            {
                printf("Prefix extends beyond Z-box. Comparing starting from R=%d\n", R + 1);
                L = i;
                while (R < concat_len && concat[R - L] == concat[R])
                    R++;
                Z[i] = R - L;
                R--;
                printf("Z[%d] computed as %d. New Z-box bounds: [L=%d, R=%d]\n", i, Z[i], L, R);
            }
        }

        if (Z[i] == pat_len)
        {
            printf("RESULT       : Match found at text index %d (since Z[%d] == %d)\n", i - pat_len - 1, i, pat_len);
            found++;
        }
        dynamic_sleep();
    }

    free(concat);
    free(Z);

    printf("\n-----------------------------------\n");
    if (found == 0)
    {
        printf("Pattern not found in the text.\n");
    }
    else
    {
        printf("Total occurrences found: %d\n", found);
    }
}

void z_algorithm_demo(void)
{
    while (1)
    {
        char text[100];
        char pattern[100];

        printf("\nZ-Algorithm String Matching Demo\n");

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
        z_algorithm_visualization(text, pattern);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("Total CPU time taken: %f seconds\n", total_t);

        add_to_history("Z-Algorithm Search", strlen(text), total_t);
    }
}
