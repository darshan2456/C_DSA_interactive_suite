#include "benchmark_chart.h"
#include "benchmark.h"
#include "sorting_algorithms_n2.h"
#include "advanced_sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static double benchmark_sorting_single(int algo_idx, int n)
{
    int* arr = malloc(n * sizeof(int));
    if (arr == NULL)
    {
        return 0.0;
    }

    for (int i = 0; i < n; i++)
    {
        arr[i] = rand() % 100000;
    }

    double start_time = benchmark_get_time();
    switch (algo_idx)
    {
        case 0: bubble_sort_optimized(arr, n); break;
        case 1: selection_sort(arr, n); break;
        case 2: insertion_sort(arr, n); break;
        case 3: shell_sort(arr, n); break;
        case 4: merge_sort(arr, n); break;
        case 5: quicksort(arr, 0, n - 1); break;
        case 6: heap_sort(arr, n); break;
        case 7: radix_sort(arr, n); break;
        case 8: bucket_sort(arr, n); break;
    }
    double end_time = benchmark_get_time();
    free(arr);

    return (end_time - start_time) * 1000.0; // convert to ms
}

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

    printf("\nGenerating complexity growth chart...\n");
    AsciiChart chart;
    chart_init(&chart, "Sorting Complexity Growth", "Input Size (N)", "Time (ms)", 80, 24);

    int chart_algos[] = {0, 2, 5, 4, 6}; // Bubble, Insertion, Quick, Merge, Heap
    char glyphs[] = {'B', 'I', 'Q', 'M', 'H'};
    const char* chart_names[] = {"Bubble", "Insertion", "Quick", "Merge", "Heap"};

    int curve_indices[5];
    for (int i = 0; i < 5; i++)
    {
        curve_indices[i] = -1;
    }

    for (int i = 0; i < 5; i++)
    {
        int algo_idx = chart_algos[i];
        if (n > 10000 && (algo_idx == 0 || algo_idx == 2))
        {
            continue; // Skip slow ones for large N
        }
        curve_indices[i] = chart_add_curve(&chart, chart_names[i], glyphs[i]);
    }

    // Run for 5 steps
    for (int step = 1; step <= 5; step++)
    {
        int current_n = step * (n / 5);
        if (current_n < 10)
        {
            current_n = 10;
        }

        for (int i = 0; i < 5; i++)
        {
            if (curve_indices[i] == -1)
            {
                continue;
            }
            double time_ms = benchmark_sorting_single(chart_algos[i], current_n);
            chart_add_point(&chart, curve_indices[i], (double)current_n, time_ms);
        }
    }

    char chart_buf[4096];
    chart_render(&chart, chart_buf, sizeof(chart_buf));
    printf("\n%s\n", chart_buf);

    free(master);
}
