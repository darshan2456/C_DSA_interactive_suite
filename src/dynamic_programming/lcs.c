#include "dp_visualizer.h"
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
        for (int j = 0; j <= n; j++)
        {
            L[i][j] = -1;
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

            visualize_dp_table_2d("Longest Common Subsequence DP Table", L, m + 1, n + 1, X, Y, i,
                                  j);
        }
    }

    visualize_dp_table_2d("Longest Common Subsequence DP Table", L, m + 1, n + 1, X, Y, m, n);

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
