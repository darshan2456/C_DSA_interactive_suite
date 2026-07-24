#include "dp_visualizer.h"
#include "dynamic_programming.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int fib_memo_size = 0;

long long fibonacci_recursive(int n, long long memo[])
{
    if (n <= 1)
        return n;
    if (memo[n] != -1)
        return memo[n];

    memo[n] = fibonacci_recursive(n - 1, memo) + fibonacci_recursive(n - 2, memo);
    if (fib_memo_size > 0)
    {
        visualize_dp_table_1d("Fibonacci Recursive (Memoized) DP Table", memo, fib_memo_size, n);
    }
    return memo[n];
}

long long fibonacci_iterative(int n)
{
    if (n <= 1)
        return n;

    long long* dp = malloc((n + 1) * sizeof(long long));
    if (dp == NULL)
    {
        printf("Memory allocation failed for DP array.\n");
        return -1;
    }
    dp[0] = 0;
    dp[1] = 1;

    for (int i = 2; i <= n; i++)
    {
        dp[i] = dp[i - 1] + dp[i - 2];
        visualize_dp_table_1d("Fibonacci Iterative (Tabulation) DP Table", dp, n + 1, i);
    }

    visualize_dp_table_1d("Fibonacci Iterative (Tabulation) DP Table", dp, n + 1, n);
    long long res = dp[n];
    free(dp);
    return res;
}
