#include "dynamic_programming.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

long long fibonacci_recursive(int n, long long memo[])
{
    if (n <= 1)
        return n;
    if (memo[n] != -1)
        return memo[n];

    memo[n] = fibonacci_recursive(n - 1, memo) + fibonacci_recursive(n - 2, memo);
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
    }

    long long res = dp[n];
    free(dp);
    return res;
}

void fibonacci_demo(void)
{
    while (1)
    {
        int n;
        int status;

        printf("\nFibonacci Sequence DP Demo\n");
        status = safe_input_int(&n, "Enter n (between 0 and 90), '-1' to exit: ", 0, 90);
        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Fibonacci demo...\n");
            return;
        }
        if (status == 0)
            continue;

        long long* memo = malloc((n + 1) * sizeof(long long));
        if (memo == NULL)
        {
            printf("Memory allocation failed for memoization array.\n");
            continue;
        }
        for (int i = 0; i <= n; i++)
        {
            memo[i] = -1;
        }

        clock_t start_m = clock();
        long long res_memo = fibonacci_recursive(n, memo);
        clock_t end_m = clock();
        double time_memo = (double)(end_m - start_m) / CLOCKS_PER_SEC;

        clock_t start_t = clock();
        long long res_tab = fibonacci_iterative(n);
        clock_t end_t = clock();
        double time_tab = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("\n--- Results ---\n");
        printf("Fibonacci(%d) via Recursive (Memoized): %lld (Time: %f seconds)\n", n, res_memo,
               time_memo);
        printf("Fibonacci(%d) via Iterative (Tabulation): %lld (Time: %f seconds)\n", n, res_tab,
               time_tab);

        free(memo);
    }
}
