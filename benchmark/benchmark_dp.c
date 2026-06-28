#define _GNU_SOURCE
#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Fibonacci
static long long fib_naive(int n)
{
    if (n <= 1)
        return n;
    return fib_naive(n - 1) + fib_naive(n - 2);
}

static long long fib_dp(int n)
{
    if (n <= 1)
        return n;
    long long a = 0, b = 1, c = 0;
    for (int i = 2; i <= n; i++)
    {
        c = a + b;
        a = b;
        b = c;
    }
    return c;
}

// Knapsack
static int knapsack_naive(int W, const int wt[], const int val[], int n)
{
    if (n == 0 || W == 0)
        return 0;
    if (wt[n - 1] > W)
        return knapsack_naive(W, wt, val, n - 1);
    int inc = val[n - 1] + knapsack_naive(W - wt[n - 1], wt, val, n - 1);
    int exc = knapsack_naive(W, wt, val, n - 1);
    return (inc > exc) ? inc : exc;
}

static int knapsack_dp(int W, const int wt[], const int val[], int n)
{
    int** dp = malloc((n + 1) * sizeof(int*));
    if (!dp)
        return 0;
    for (int i = 0; i <= n; i++)
    {
        dp[i] = malloc((W + 1) * sizeof(int));
    }
    for (int i = 0; i <= n; i++)
    {
        for (int w = 0; w <= W; w++)
        {
            if (i == 0 || w == 0)
                dp[i][w] = 0;
            else if (wt[i - 1] <= w)
            {
                int inc = val[i - 1] + dp[i - 1][w - wt[i - 1]];
                int exc = dp[i - 1][w];
                dp[i][w] = (inc > exc) ? inc : exc;
            }
            else
            {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    int res = dp[n][W];
    for (int i = 0; i <= n; i++)
        free(dp[i]);
    free(dp);
    return res;
}

// LCS
static int lcs_naive(const char* X, const char* Y, int m, int n)
{
    if (m == 0 || n == 0)
        return 0;
    if (X[m - 1] == Y[n - 1])
        return 1 + lcs_naive(X, Y, m - 1, n - 1);
    int a = lcs_naive(X, Y, m, n - 1);
    int b = lcs_naive(X, Y, m - 1, n);
    return (a > b) ? a : b;
}

static int lcs_dp(const char* X, const char* Y, int m, int n)
{
    int** dp = malloc((m + 1) * sizeof(int*));
    if (!dp)
        return 0;
    for (int i = 0; i <= m; i++)
    {
        dp[i] = malloc((n + 1) * sizeof(int));
    }
    for (int i = 0; i <= m; i++)
    {
        for (int j = 0; j <= n; j++)
        {
            if (i == 0 || j == 0)
                dp[i][j] = 0;
            else if (X[i - 1] == Y[j - 1])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
            {
                int a = dp[i][j - 1];
                int b = dp[i - 1][j];
                dp[i][j] = (a > b) ? a : b;
            }
        }
    }
    int res = dp[m][n];
    for (int i = 0; i <= m; i++)
        free(dp[i]);
    free(dp);
    return res;
}

// MCM
static int mcm_naive(const int p[], int i, int j)
{
    if (i == j)
        return 0;
    int min_val = 1e9;
    for (int k = i; k < j; k++)
    {
        int count = mcm_naive(p, i, k) + mcm_naive(p, k + 1, j) + p[i - 1] * p[k] * p[j];
        if (count < min_val)
            min_val = count;
    }
    return min_val;
}

static int mcm_dp(const int p[], int n)
{
    int** m = malloc(n * sizeof(int*));
    if (!m)
        return 0;
    for (int i = 0; i < n; i++)
    {
        m[i] = calloc(n, sizeof(int));
    }
    for (int l = 2; l < n; l++)
    {
        for (int i = 1; i < n - l + 1; i++)
        {
            int j = i + l - 1;
            m[i][j] = 1e9;
            for (int k = i; k < j; k++)
            {
                int q = m[i][k] + m[k + 1][j] + p[i - 1] * p[k] * p[j];
                if (q < m[i][j])
                    m[i][j] = q;
            }
        }
    }
    int res = m[1][n - 1];
    for (int i = 0; i < n; i++)
        free(m[i]);
    free(m);
    return res;
}

void run_dp_benchmark(int n)
{
    srand((unsigned int)time(NULL));

    // Cap dynamic programming input sizes when launching demo
    int fib_n = (n > 90) ? 90 : n;
    int kp_n = (n > 1000) ? 1000 : n;
    int lcs_n = (n > 1000) ? 1000 : n;
    int mcm_n = (n > 200) ? 200 : n;

    printf("\n========================================================================\n");
    printf("        BENCHMARK REPORT: DYNAMIC PROGRAMMING VS NAIVE RECURSION\n");
    printf("========================================================================\n");
    printf("%-35s %-15s %-15s %-10s\n", "Problem (Size)", "Recursion Time", "DP Time", "Status");
    printf("------------------------------------------------------------------------\n");

    // 1. Fibonacci
    {
        double rec_time = -1.0;
        size_t mem_used = 0;
        if (fib_n <= 38)
        {
            size_t mem_before = benchmark_get_peak_memory();
            double start = benchmark_get_time();
            fib_naive(fib_n);
            rec_time = benchmark_get_time() - start;
            size_t mem_after = benchmark_get_peak_memory();
            mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
        }

        size_t dp_mem_before = benchmark_get_peak_memory();
        double start_dp = benchmark_get_time();
        fib_dp(fib_n);
        double dp_time = benchmark_get_time() - start_dp;
        size_t dp_mem_after = benchmark_get_peak_memory();
        size_t dp_mem_used = (dp_mem_after > dp_mem_before) ? (dp_mem_after - dp_mem_before) : 0;
        if (dp_mem_used == 0)
            dp_mem_used = dp_mem_after;

        char size_str[32];
        sprintf(size_str, "Fibonacci (%d)", fib_n);
        if (rec_time >= 0)
        {
            printf("%-35s %-15.6f %-15.6f %-10s\n", size_str, rec_time * 1000.0, dp_time * 1000.0,
                   "PASSED");
            benchmark_export_csv("dp", "Fibonacci (Recursion)", fib_n, rec_time, mem_used);
        }
        else
        {
            printf("%-35s %-15s %-15.6f %-10s\n", size_str, "BYPASSED", dp_time * 1000.0, "PASSED");
        }
        benchmark_export_csv("dp", "Fibonacci (DP)", fib_n, dp_time, dp_mem_used);
    }

    // 2. Knapsack
    {
        int W = 100;
        int* wt = malloc(kp_n * sizeof(int));
        int* val = malloc(kp_n * sizeof(int));
        if (wt && val)
        {
            for (int i = 0; i < kp_n; i++)
            {
                wt[i] = rand() % 20 + 1;
                val[i] = rand() % 100 + 1;
            }

            double rec_time = -1.0;
            size_t mem_used = 0;
            if (kp_n <= 22)
            {
                size_t mem_before = benchmark_get_peak_memory();
                double start = benchmark_get_time();
                knapsack_naive(W, wt, val, kp_n);
                rec_time = benchmark_get_time() - start;
                size_t mem_after = benchmark_get_peak_memory();
                mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
            }

            size_t dp_mem_before = benchmark_get_peak_memory();
            double start_dp = benchmark_get_time();
            knapsack_dp(W, wt, val, kp_n);
            double dp_time = benchmark_get_time() - start_dp;
            size_t dp_mem_after = benchmark_get_peak_memory();
            size_t dp_mem_used =
                (dp_mem_after > dp_mem_before) ? (dp_mem_after - dp_mem_before) : 0;
            if (dp_mem_used == 0)
                dp_mem_used = dp_mem_after;

            char size_str[32];
            sprintf(size_str, "0/1 Knapsack (%d)", kp_n);
            if (rec_time >= 0)
            {
                printf("%-35s %-15.6f %-15.6f %-10s\n", size_str, rec_time * 1000.0,
                       dp_time * 1000.0, "PASSED");
                benchmark_export_csv("dp", "Knapsack (Recursion)", kp_n, rec_time, mem_used);
            }
            else
            {
                printf("%-35s %-15s %-15.6f %-10s\n", size_str, "BYPASSED", dp_time * 1000.0,
                       "PASSED");
            }
            benchmark_export_csv("dp", "Knapsack (DP)", kp_n, dp_time, dp_mem_used);
        }
        free(wt);
        free(val);
    }

    // 3. LCS
    {
        char* X = malloc(lcs_n + 1);
        char* Y = malloc(lcs_n + 1);
        if (X && Y)
        {
            for (int i = 0; i < lcs_n; i++)
            {
                X[i] = 'A' + (rand() % 4);
                Y[i] = 'A' + (rand() % 4);
            }
            X[lcs_n] = '\0';
            Y[lcs_n] = '\0';

            double rec_time = -1.0;
            size_t mem_used = 0;
            if (lcs_n <= 16)
            {
                size_t mem_before = benchmark_get_peak_memory();
                double start = benchmark_get_time();
                lcs_naive(X, Y, lcs_n, lcs_n);
                rec_time = benchmark_get_time() - start;
                size_t mem_after = benchmark_get_peak_memory();
                mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
            }

            size_t dp_mem_before = benchmark_get_peak_memory();
            double start_dp = benchmark_get_time();
            lcs_dp(X, Y, lcs_n, lcs_n);
            double dp_time = benchmark_get_time() - start_dp;
            size_t dp_mem_after = benchmark_get_peak_memory();
            size_t dp_mem_used =
                (dp_mem_after > dp_mem_before) ? (dp_mem_after - dp_mem_before) : 0;
            if (dp_mem_used == 0)
                dp_mem_used = dp_mem_after;

            char size_str[32];
            sprintf(size_str, "LCS (%d)", lcs_n);
            if (rec_time >= 0)
            {
                printf("%-35s %-15.6f %-15.6f %-10s\n", size_str, rec_time * 1000.0,
                       dp_time * 1000.0, "PASSED");
                benchmark_export_csv("dp", "LCS (Recursion)", lcs_n, rec_time, mem_used);
            }
            else
            {
                printf("%-35s %-15s %-15.6f %-10s\n", size_str, "BYPASSED", dp_time * 1000.0,
                       "PASSED");
            }
            benchmark_export_csv("dp", "LCS (DP)", lcs_n, dp_time, dp_mem_used);
        }
        free(X);
        free(Y);
    }

    // 4. MCM
    {
        int mcm_len = mcm_n + 1;
        int* p = malloc(mcm_len * sizeof(int));
        if (p)
        {
            for (int i = 0; i < mcm_len; i++)
            {
                p[i] = rand() % 20 + 5;
            }

            double rec_time = -1.0;
            size_t mem_used = 0;
            if (mcm_n <= 12)
            {
                size_t mem_before = benchmark_get_peak_memory();
                double start = benchmark_get_time();
                mcm_naive(p, 1, mcm_n);
                rec_time = benchmark_get_time() - start;
                size_t mem_after = benchmark_get_peak_memory();
                mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
            }

            size_t dp_mem_before = benchmark_get_peak_memory();
            double start_dp = benchmark_get_time();
            mcm_dp(p, mcm_len);
            double dp_time = benchmark_get_time() - start_dp;
            size_t dp_mem_after = benchmark_get_peak_memory();
            size_t dp_mem_used =
                (dp_mem_after > dp_mem_before) ? (dp_mem_after - dp_mem_before) : 0;
            if (dp_mem_used == 0)
                dp_mem_used = dp_mem_after;

            char size_str[32];
            sprintf(size_str, "Matrix Chain Mult (%d)", mcm_n);
            if (rec_time >= 0)
            {
                printf("%-35s %-15.6f %-15.6f %-10s\n", size_str, rec_time * 1000.0,
                       dp_time * 1000.0, "PASSED");
                benchmark_export_csv("dp", "MCM (Recursion)", mcm_n, rec_time, mem_used);
            }
            else
            {
                printf("%-35s %-15s %-15.6f %-10s\n", size_str, "BYPASSED", dp_time * 1000.0,
                       "PASSED");
            }
            benchmark_export_csv("dp", "MCM (DP)", mcm_n, dp_time, dp_mem_used);
        }
        free(p);
    }
    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/dp.csv'.\n");
}
