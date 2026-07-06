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
        }
    }

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

void mcm_demo(void)
{
    int num_matrices;
    while (1)
    {
        printf("\n\nMatrix Chain Multiplication demo");
        int mcm_status = safe_input_int(&num_matrices,
                                        "\nenter the number of matrices "
                                        "(between 1 and 15), enter '-1' to exit:- ",
                                        1, 15);

        if (mcm_status == 0)
        {
            continue;
        }

        if (mcm_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Matrix Chain Multiplication demo....\n");
            return;
        }

        int n = num_matrices + 1;
        int* p = malloc(sizeof(int) * n);
        if (p == NULL)
        {
            printf("Memory allocation failed for dimensions array.\n");
            return;
        }

        printf("\nEnter the dimensions:\n");
        for (int i = 0; i < n; i++)
        {
        retry:
            if (i == 0)
            {
                printf("enter dimension p0 (row count of matrix A1, between 1 and 500):- ");
            }
            else
            {
                printf("enter dimension p%d (column count of matrix A%d, between 1 and 500):- ", i,
                       i);
            }

            int dim_status = safe_input_int(&p[i], NULL, 1, 500);

            if (dim_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Matrix Chain Multiplication demo.....\n");
                free(p);
                return;
            }

            if (dim_status == 0)
            {
                goto retry;
            }
        }

        clock_t start_t = clock();
        int min_multiplications = matrix_chain_order(p, n);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("\nMinimum Scalar Multiplications required: %d", min_multiplications);
        printf("\nTotal CPU time taken:- %f seconds", total_t);

        free(p);
    }
}
