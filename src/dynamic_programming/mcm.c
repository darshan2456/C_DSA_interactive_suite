#include "config.h"
#include "dp_visualizer.h"
#include "dynamic_programming.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void print_parenthesization(int i, int j, int n, int* bracket, char* name)
{
    if (i == j)
    {
        printf("%c", name[i - 1]);
        return;
    }
    printf("(");
    int split = bracket[i * n + j];
    print_parenthesization(i, split, n, bracket, name);
    print_parenthesization(split + 1, j, n, bracket, name);
    printf(")");
}

int matrix_chain_order(int p[], int n)
{
    if (n < 2)
    {
        return 0;
    }

    // Allocate 1D arrays of size n*n to represent 2D tables
    int* m = calloc(n * n, sizeof(int));
    int* s = calloc(n * n, sizeof(int));

    if (m == NULL || s == NULL)
    {
        free(m);
        free(s);
        printf("Memory allocation failed in matrix_chain_order\n");
        return -1;
    }

    // Solve the MCM DP problem
    for (int L = 2; L < n; L++)
    {
        for (int i = 1; i < n - L + 1; i++)
        {
            int j = i + L - 1;
            m[i * n + j] = INT_MAX;
            for (int k = i; k <= j - 1; k++)
            {
                int q = m[i * n + k] + m[(k + 1) * n + j] + p[i - 1] * p[k] * p[j];
                if (q < m[i * n + j])
                {
                    m[i * n + j] = q;
                    s[i * n + j] = k;
                }
            }
            visualize_mcm_tables(m, s, n, i, j);
        }
    }

    if (!is_terminal_interactive())
    {
        // Print m-table
        printf("\nm-table (DP Lookup Table - Minimum Multiplications):\n");
        for (int i = 1; i < n; i++)
        {
            for (int j = 1; j < n; j++)
            {
                if (i > j)
                {
                    printf("%-10s", "-");
                }
                else
                {
                    printf("%-10d", m[i * n + j]);
                }
            }
            printf("\n");
        }

        // Print s-table
        printf("\ns-table (Bracket/Parenthesization Split Table):\n");
        for (int i = 1; i < n; i++)
        {
            for (int j = 1; j < n; j++)
            {
                if (i >= j)
                {
                    printf("%-5s", "-");
                }
                else
                {
                    printf("%-5d", s[i * n + j]);
                }
            }
            printf("\n");
        }
    }
    else
    {
        visualize_mcm_tables(m, s, n, 1, n - 1);
    }

    int min_multiplications = m[1 * n + (n - 1)];

    // Construct matrix names: A, B, C, ...
    char* names = malloc(sizeof(char) * n);
    if (names != NULL)
    {
        for (int i = 0; i < n - 1; i++)
        {
            names[i] = 'A' + i;
        }
        names[n - 1] = '\0';

        printf("\nOptimal Parenthesization: ");
        print_parenthesization(1, n - 1, n, s, names);
        printf("\n");

        free(names);
    }

    free(m);
    free(s);

    return min_multiplications;
}
