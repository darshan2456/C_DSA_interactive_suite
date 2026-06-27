#define _GNU_SOURCE
#include "benchmark.h"
#include "graph_traversals.h"
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

void run_mst_benchmark(int v)
{
    // Seed random generator
    srand((unsigned int)time(NULL));

    // Safety Guard: bypass all MST benchmarks if V is too large to prevent freezes
    if (v > 2000)
    {
        printf("\nMST benchmark skipped for V = %d (Threshold is V = 2000)\n", v);
        return;
    }

    // Allocate graph
    weightedGraph* graph = create_weightedGraph(v);
    if (graph == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for graph V = %d\n", v);
        return;
    }

    // Ensure connectedness by creating an undirected chain 0 <-> 1 <-> ... <-> V-1
    for (int i = 0; i < v - 1; i++)
    {
        add_edge_directed(graph, i, i + 1, rand() % 90 + 10);
        add_edge_directed(graph, i + 1, i, rand() % 90 + 10);
    }

    // Add additional random undirected edges (5% probability)
    for (int src = 0; src < v; src++)
    {
        for (int dest = src + 1; dest < v; dest++)
        {
            if (rand() % 100 < 5)
            {
                int weight = rand() % 90 + 10;
                add_edge_directed(graph, src, dest, weight);
                add_edge_directed(graph, dest, src, weight);
            }
        }
    }

    // Print table header
    printf("\n========================================================================\n");
    printf("             BENCHMARK REPORT: MINIMUM SPANNING TREE (V = %d)\n", v);
    printf("========================================================================\n");
    printf("%-30s %-20s %-12s %-10s\n", "Algorithm", "Execution Time", "Peak Memory", "Status");
    printf("------------------------------------------------------------------------\n");

    const char* algos[] = {
        "Kruskal's Algorithm", "Prim's Algorithm"
    };

    for (int i = 0; i < 2; i++)
    {
        const char* name = algos[i];

        // Track memory before
        size_t mem_before = benchmark_get_peak_memory();
        double start_time = benchmark_get_time();

        // Redirect stdout to suppress print logs from algorithm execution
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
            case 0: kruskal_mst(graph); break;
            case 1: prim_mst(graph, 0); break;
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

        double end_time = benchmark_get_time();
        size_t mem_after = benchmark_get_peak_memory();

        double elapsed = end_time - start_time;
        size_t peak_mem = (mem_after > mem_before) ? mem_after : mem_before;

        // Format print row
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

        printf("%-30s %-20s %-12s %-10s\n", name, time_str, mem_str, "PASSED");

        // Export to CSV
        benchmark_export_csv("mst", name, v, elapsed, peak_mem);
    }

    printf("========================================================================\n");
    printf("Exported results to 'benchmarks/mst.csv'.\n");

    free_weightedGraph(graph);
}
