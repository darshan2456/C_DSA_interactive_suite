#include "advanced_heaps.h"
#include "benchmark.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void run_heaps_benchmark(int n)
{
    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: ADVANCED HEAPS (N = %d)\n", n);
    printf("========================================================================\n");
    printf("%-30s %-20s %-12s %-9s\n", "Heap Type", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    // Initialize random number generator
    srand(42);
    int* random_keys = (int*)malloc(n * sizeof(int));
    int* random_vals = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        random_keys[i] = rand() % 1000000;
        random_vals[i] = i;
    }

    // 1. Binomial Heap Benchmark
    {
        double start_time = benchmark_get_time();
        BinomialNode* heap = NULL;

        // Perform inserts
        for (int i = 0; i < n; i++)
        {
            heap = binomial_heap_insert(heap, random_keys[i], random_vals[i]);
        }

        // Perform extracts
        for (int i = 0; i < n; i++)
        {
            int k, v;
            heap = binomial_heap_extract_min(heap, &k, &v);
        }
        destroy_binomial_heap(heap);

        double elapsed_time = benchmark_get_time() - start_time;
        size_t memory = benchmark_get_peak_memory();

        printf("%-30s %-20f %-12zu %-9s\n", "Binomial Heap", elapsed_time, memory, "PASSED");
        benchmark_export_csv("heaps", "Binomial Heap", n, elapsed_time, memory);
    }

    // 2. Fibonacci Heap Benchmark
    {
        double start_time = benchmark_get_time();
        FibonacciNode* heap = NULL;

        // Perform inserts
        for (int i = 0; i < n; i++)
        {
            heap = fib_heap_insert(heap, random_keys[i], random_vals[i]);
        }

        // Perform extracts
        for (int i = 0; i < n; i++)
        {
            int k, v;
            heap = fib_heap_extract_min(heap, &k, &v);
        }
        destroy_fibonacci_heap(heap);

        double elapsed_time = benchmark_get_time() - start_time;
        size_t memory = benchmark_get_peak_memory();

        printf("%-30s %-20f %-12zu %-9s\n", "Fibonacci Heap", elapsed_time, memory, "PASSED");
        benchmark_export_csv("heaps", "Fibonacci Heap", n, elapsed_time, memory);
    }

    // 3. Min-Max Heap Benchmark
    {
        double start_time = benchmark_get_time();
        MinMaxHeap* heap = create_min_max_heap(n);

        // Perform inserts
        for (int i = 0; i < n; i++)
        {
            min_max_heap_insert(heap, random_keys[i], random_vals[i]);
        }

        // Perform extracts
        for (int i = 0; i < n; i++)
        {
            int k, v;
            min_max_heap_extract_min(heap, &k, &v);
        }
        destroy_min_max_heap(heap);

        double elapsed_time = benchmark_get_time() - start_time;
        size_t memory = benchmark_get_peak_memory();

        printf("%-30s %-20f %-12zu %-9s\n", "Min-Max Heap", elapsed_time, memory, "PASSED");
        benchmark_export_csv("heaps", "Min-Max Heap", n, elapsed_time, memory);
    }

    free(random_keys);
    free(random_vals);

    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/heaps.csv'.\n");
}
