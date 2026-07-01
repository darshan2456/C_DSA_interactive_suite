#define _GNU_SOURCE
#include "advanced_graph_algorithms.h"
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

static weightedGraph* generate_flow_network(int V, double edge_density)
{
    weightedGraph* graph = create_weightedGraph(V);
    if (graph == NULL)
        return NULL;

    for (int i = 0; i < V - 1; i++)
    {
        add_edge_directed(graph, i, i + 1, rand() % 90 + 10);
    }

    for (int src = 0; src < V; src++)
    {
        for (int dest = 0; dest < V; dest++)
        {
            if (src != dest)
            {
                if ((double)rand() / RAND_MAX < edge_density)
                {
                    add_edge_directed(graph, src, dest, rand() % 90 + 10);
                }
            }
        }
    }
    return graph;
}

void run_flow_benchmark(int v)
{
    srand((unsigned int)time(NULL));

    if (v > 1000)
    {
        printf("\nMaximum Flow benchmark skipped for V = %d (Threshold is V = 1000)\n", v);
        return;
    }

    const char* network_types[] = {"Sparse (Density = 5%)", "Dense (Density = 30%)"};
    double densities[] = {0.05, 0.30};

    for (int net = 0; net < 2; net++)
    {
        weightedGraph* graph = generate_flow_network(v, densities[net]);
        if (graph == NULL)
        {
            fprintf(stderr, "Error: Memory allocation failed for graph V = %d\n", v);
            return;
        }

        printf("\n========================================================================\n");
        printf("       BENCHMARK REPORT: MAXIMUM FLOW [%s] (V = %d)\n", network_types[net], v);
        printf("========================================================================\n");
        printf("%-30s %-20s %-12s %-10s\n", "Algorithm", "Execution Time", "Peak Memory", "Status");
        printf("------------------------------------------------------------------------\n");

        const char* algos[] = {"Ford-Fulkerson (DFS)", "Edmonds-Karp (BFS)",
                               "Dinic's (Level Graph)"};

        for (int i = 0; i < 3; i++)
        {
            const char* name = algos[i];
            int skip = 0;

            if (v > 300 && net == 1 && i < 2)
            {
                skip = 1;
            }

            if (skip)
            {
                printf("%-30s %-20s %-12s %-10s\n", name, "Skipped (Dense/Large)", "N/A",
                       "SKIPPED");
                continue;
            }

            size_t mem_before = benchmark_get_peak_memory();
            double start_time = benchmark_get_time();

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

            if (i == 0)
            {
                ford_fulkerson(graph, 0, v - 1);
            }
            else if (i == 1)
            {
                edmonds_karp(graph, 0, v - 1);
            }
            else if (i == 2)
            {
                dinic(graph, 0, v - 1);
            }

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

            char csv_cat[64];
            snprintf(csv_cat, sizeof(csv_cat), "flow_%s", net == 0 ? "sparse" : "dense");
            benchmark_export_csv(csv_cat, name, v, elapsed, peak_mem);
        }

        printf("========================================================================\n");
        free_weightedGraph(graph);
    }
}
