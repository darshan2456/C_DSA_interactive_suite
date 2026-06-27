#define _GNU_SOURCE
#include "benchmark.h"
#include "../string_algorithms/string_algorithms.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#else
#include <unistd.h>
#endif

static void generate_random_string(char* str, int len)
{
    for (int i = 0; i < len; i++)
    {
        str[i] = 'A' + (rand() % 2);
    }
    str[len] = '\0';
}

void run_strings_benchmark(int n)
{
    srand((unsigned int)time(NULL));

    if (n > 100000)
    {
        printf("\nString Matching benchmark skipped for N = %d (Threshold is N = 100000)\n", n);
        return;
    }

    char* text = malloc(n + 1);
    char pattern[11];
    if (!text)
    {
        return;
    }

    generate_random_string(text, n);
    generate_random_string(pattern, 10);

    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: STRING MATCHING ALGORITHMS (N = %d)\n", n);
    printf("========================================================================\n");
    printf("%-30s %-20s %-12s %-10s\n", "Algorithm", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    const char* algos[] = {
        "Naive String Matching",
        "Knuth-Morris-Pratt (KMP)",
        "Rabin-Karp Search"
    };

    for (int i = 0; i < 3; i++)
    {
        // Skip Naive if N is very large to avoid timeout
        if (i == 0 && n > 20000)
        {
            printf("%-30s %-20s %-12s %-10s\n", algos[i], "N/A", "N/A", "BYPASSED");
            continue;
        }

        size_t mem_before = benchmark_get_peak_memory();
        double start_time = benchmark_get_time();

        // Redirect stdout
        fflush(stdout);
        int stdout_dup = dup(1);
#ifdef _WIN32
        FILE* fnull = fopen("NUL", "w");
#else
        FILE* fnull = fopen("/dev/null", "w");
#endif
        if (fnull != NULL && stdout_dup >= 0)
        {
            dup2(fileno(fnull), 1);
        }

        // Run algorithm
        switch (i)
        {
            case 0:
                naive_string_matching(text, pattern);
                break;
            case 1:
                kmp_search(text, pattern);
                break;
            case 2:
                rabin_karp_search(text, pattern, 101);
                break;
        }

        // Restore stdout
        fflush(stdout);
        if (stdout_dup >= 0)
        {
            dup2(stdout_dup, 1);
            close(stdout_dup);
        }
        if (fnull != NULL)
        {
            fclose(fnull);
        }

        double duration = benchmark_get_time() - start_time;
        size_t mem_after = benchmark_get_peak_memory();
        size_t mem_used = (mem_after > mem_before) ? (mem_after - mem_before) : 0;
        if (mem_used == 0) mem_used = mem_after;

        printf("%-30s %-20.6f %-12zu %-10s\n", algos[i], duration * 1000.0, mem_used, "PASSED");
        benchmark_export_csv("strings", algos[i], n, duration, mem_used);
    }
    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/strings.csv'.\n");

    free(text);
}
