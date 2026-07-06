#include "dynamic_programming.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int max(int a, int b)
{
    return (a > b) ? a : b;
}

int lcs(char* X, char* Y, int m, int n)
{
    int** L = malloc((m + 1) * sizeof(int*));
    if (L == NULL)
    {
        printf("Memory allocation failed for DP table.\n");
        return -1;
    }
    for (int i = 0; i <= m; i++)
    {
        L[i] = malloc((n + 1) * sizeof(int));
        if (L[i] == NULL)
        {
            printf("Memory allocation failed for DP table row %d.\n", i);
            for (int j = 0; j < i; j++)
            {
                free(L[j]);
            }
            free(L);
            return -1;
        }
    }

    int i, j;
    for (i = 0; i <= m; i++)
    {
        for (j = 0; j <= n; j++)
        {
            if (i == 0 || j == 0)
                L[i][j] = 0;
            else if (X[i - 1] == Y[j - 1])
                L[i][j] = L[i - 1][j - 1] + 1;
            else
                L[i][j] = max(L[i - 1][j], L[i][j - 1]);
        }
    }

    printf("\n--- LCS DP Table ---\n");
    for (i = 0; i <= m; i++)
    {
        for (j = 0; j <= n; j++)
        {
            printf("%3d ", L[i][j]);
        }
        printf("\n");
    }

    int index = L[m][n];
    char lcs_str[index + 1];
    lcs_str[index] = '\0';

    i = m;
    j = n;
    while (i > 0 && j > 0)
    {
        if (X[i - 1] == Y[j - 1])
        {
            lcs_str[index - 1] = X[i - 1];
            i--;
            j--;
            index--;
        }
        else if (L[i - 1][j] > L[i][j - 1])
            i--;
        else
            j--;
    }

    printf("\nTraceback: Longest Common Subsequence is \"%s\"\n", lcs_str);

    int res = L[m][n];

    for (i = 0; i <= m; i++)
    {
        free(L[i]);
    }
    free(L);

    return res;
}

void lcs_demo(void)
{
    while (1)
    {
        char X[100];
        char Y[100];

        printf("\nLCS Demo\n");

        int status_X =
            safe_input_string(X, "Enter first string (no spaces, max 99 chars), or 'X' to exit: ");
        if (status_X == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting LCS demo...\n");
            return;
        }

        int status_Y =
            safe_input_string(Y, "Enter second string (no spaces, max 99 chars), or 'X' to exit: ");
        if (status_Y == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting LCS demo...\n");
            return;
        }

        int m = strlen(X);
        int n = strlen(Y);

        clock_t start_t = clock();
        int lcs_len = lcs(X, Y, m, n);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("Length of LCS is: %d\n", lcs_len);
        printf("Total CPU time taken: %f seconds\n", total_t);
    }
}
