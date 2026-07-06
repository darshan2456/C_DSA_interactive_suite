#include "dynamic_programming.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int max(int a, int b)
{
    return (a > b) ? a : b;
}

int knapsack(int W, int wt[], int val[], int n)
{
    int i, w;
    int** K = malloc((n + 1) * sizeof(int*));
    if (K == NULL)
    {
        printf("Memory allocation failed for DP table.\n");
        return -1;
    }
    for (i = 0; i <= n; i++)
    {
        K[i] = malloc((W + 1) * sizeof(int));
        if (K[i] == NULL)
        {
            printf("Memory allocation failed for DP table row %d.\n", i);
            for (int j = 0; j < i; j++)
            {
                free(K[j]);
            }
            free(K);
            return -1;
        }
    }

    for (i = 0; i <= n; i++)
    {
        for (w = 0; w <= W; w++)
        {
            if (i == 0 || w == 0)
                K[i][w] = 0;
            else if (wt[i - 1] <= w)
                K[i][w] = max(val[i - 1] + K[i - 1][w - wt[i - 1]], K[i - 1][w]);
            else
                K[i][w] = K[i - 1][w];
        }
    }

    printf("\n--- 0/1 Knapsack DP Table ---\n");
    for (i = 0; i <= n; i++)
    {
        for (w = 0; w <= W; w++)
        {
            printf("%4d ", K[i][w]);
        }
        printf("\n");
    }
    printf("-----------------------------\n");

    int res = K[n][W];

    for (i = 0; i <= n; i++)
    {
        free(K[i]);
    }
    free(K);

    return res;
}

void knapsack_demo(void)
{
    while (1)
    {
        int n, W;
        int status;

        printf("\n0/1 Knapsack Demo\n");
        status =
            safe_input_int(&n, "Enter number of items (between 1 and 20), '-1' to exit: ", 1, 20);
        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Knapsack demo...\n");
            return;
        }
        if (status == 0)
            continue;

        status = safe_input_int(
            &W, "Enter knapsack capacity (between 1 and 100), '-1' to exit: ", 1, 100);
        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Knapsack demo...\n");
            return;
        }
        if (status == 0)
            continue;

        int val[n];
        int wt[n];

        for (int i = 0; i < n; i++)
        {
            printf("\nItem %d:\n", i + 1);
        retry_wt:
            status = safe_input_int(&wt[i], "  Enter weight (1 to 100), '-1' to exit: ", 1, 100);
            if (status == INPUT_EXIT_SIGNAL)
                return;
            if (status == 0)
                goto retry_wt;

        retry_val:
            status = safe_input_int(&val[i], "  Enter value (1 to 1000), '-1' to exit: ", 1, 1000);
            if (status == INPUT_EXIT_SIGNAL)
                return;
            if (status == 0)
                goto retry_val;
        }

        clock_t start_t = clock();
        int max_val = knapsack(W, wt, val, n);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("\nMaximum value that can be put in a knapsack of capacity %d is: %d\n", W, max_val);
        printf("Total CPU time taken: %f seconds\n", total_t);
    }
}
