#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "benchmark.h"

void test_graphs_benchmark_run(void)
{
    // Clean up old files
    remove("benchmarks/graphs.csv");

    // Run benchmark with size V = 100
    run_graphs_benchmark(100);

    // Verify CSV output exists
    FILE* file = fopen("benchmarks/graphs.csv", "r");
    assert(file != NULL);

    char line[256];
    int row_count = 0;

    // Read header
    char* res = fgets(line, sizeof(line), file);
    assert(res != NULL);
    assert(strstr(line, "Algorithm,Input Size,Execution Time (Seconds),Peak Memory (KB),Timestamp") != NULL);

    // Count data rows
    while (fgets(line, sizeof(line), file))
    {
        row_count++;
    }
    fclose(file);

    // There should be exactly 4 data rows (Dijkstra, Bellman-Ford, A*, Greedy Best-First Search)
    assert(row_count == 4);

    // Clean up
    remove("benchmarks/graphs.csv");
    printf("test_graphs_benchmark_run passed successfully.\n");
}

int main(void)
{
    test_graphs_benchmark_run();
    printf("All graph benchmark tests passed successfully!\n");
    return 0;
}
