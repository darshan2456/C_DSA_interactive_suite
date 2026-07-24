#include "config.h"
#include "dp_visualizer.h"
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
        for (int j = 0; j <= W; j++)
        {
            K[i][j] = -1;
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

            visualize_dp_table_2d("0/1 Knapsack DP Table", K, n + 1, W + 1, NULL, NULL, i, w);
        }
    }

    if (!is_terminal_interactive())
    {
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
    }
    else
    {
        visualize_dp_table_2d("0/1 Knapsack DP Table", K, n + 1, W + 1, NULL, NULL, n, W);
    }

    int res = K[n][W];

    for (i = 0; i <= n; i++)
    {
        free(K[i]);
    }
    free(K);

    return res;
}
