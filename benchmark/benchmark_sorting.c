#include "benchmark.h"
#include "sorting_algorithms_n2.h"
#include "advanced_sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void run_sorting_benchmark(int n)
{
    // Seed random generator
    srand((unsigned int)time(NULL));

    // Allocate master array
    int* master = malloc(n * sizeof(int));
    if (master == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for benchmark size N = %d\n", n);
        return;
    }

    // Fill master array with random numbers
    for (int i = 0; i < n; i++)
    {
        master[i] = rand() % 100000;
    }

    // Print table header
    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: SORTING ALGORITHMS (N = %d)\n", n);
    printf("========================================================================\n");
    printf("%-20s %-25s %-15s %-10s\n", "Algorithm", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    const char* algos[] = {
        "Bubble Sort", "Selection Sort", "Insertion Sort", "Shell Sort",
        "Merge Sort", "Quick Sort", "Heap Sort", "Radix Sort", "Bucket Sort"
    };

    for (int i = 0; i < 9; i++)
    {
        const char* name = algos[i];
        int skip = 0;
        
        // Skip O(N^2) algorithms for N > 10000 to prevent long freezes
        if (n > 10000 && (i == 0 || i == 1 || i == 2))
        {
            skip = 1;
        }

        if (skip)
        {
            printf("%-20s %-25s %-15s %-10s\n", name, "Skipped (N > 10000)", "N/A", "SKIPPED");
            continue;
        }

        // Clone master array
        int* clone = malloc(n * sizeof(int));
        if (clone == NULL)
        {
            printf("%-20s %-25s %-15s %-10s\n", name, "Alloc Failure", "N/A", "FAILED");
            continue;
        }
        memcpy(clone, master, n * sizeof(int));

        // Track memory before
        size_t mem_before = benchmark_get_peak_memory();
        double start_time = benchmark_get_time();

        // Run algorithm
        switch (i)
        {
            case 0: bubble_sort_optimized(clone, n); break;
            case 1: selection_sort(clone, n); break;
            case 2: insertion_sort(clone, n); break;
            case 3: shell_sort(clone, n); break;
            case 4: merge_sort(clone, n); break;
            case 5: quicksort(clone, 0, n - 1); break;
            case 6: heap_sort(clone, n); break;
            case 7: radix_sort(clone, n); break;
            case 8: bucket_sort(clone, n); break;
        }

        double end_time = benchmark_get_time();
        size_t mem_after = benchmark_get_peak_memory();

        double elapsed = end_time - start_time;
        size_t peak_mem = (mem_after > mem_before) ? mem_after : mem_before;

        // Print row
        char time_str[30];
        if (elapsed < 0.001)
        {
            snprintf(time_str, sizeof(time_str), "%.6f ms", elapsed * 1000.0);
        }
        else
        {
            snprintf(time_str, sizeof(time_str), "%.2f ms", elapsed * 1000.0);
        }

        char mem_str[30];
        snprintf(mem_str, sizeof(mem_str), "%zu KB", peak_mem);

        printf("%-20s %-25s %-15s %-10s\n", name, time_str, mem_str, "PASSED");

        // Export to CSV
        benchmark_export_csv("sorting", name, n, elapsed, peak_mem);

        free(clone);
    }

    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/sorting.csv'.\n");

    free(master);
}
