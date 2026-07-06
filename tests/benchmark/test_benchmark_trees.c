#include "benchmark.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_trees_benchmark_run(void)
{
    // Clean up old files
    remove("benchmarks/trees.csv");

    // Run benchmark with size N = 50
    run_trees_benchmark(50);

    // Verify CSV output exists
    FILE* file = fopen("benchmarks/trees.csv", "r");
    assert(file != NULL);

    char line[256];
    int row_count = 0;

    // Read header
    char* res = fgets(line, sizeof(line), file);
    assert(res != NULL);
    assert(
        strstr(line, "Algorithm,Input Size,Execution Time (Seconds),Peak Memory (KB),Timestamp") !=
        NULL);

    // Count data rows
    while (fgets(line, sizeof(line), file))
    {
        row_count++;
    }
    fclose(file);

    // There should be exactly 8 data rows
    assert(row_count == 8);

    // Clean up
    remove("benchmarks/trees.csv");
    printf("test_trees_benchmark_run passed successfully.\n");
}

int main(void)
{
    test_trees_benchmark_run();
    printf("All Trees Performance benchmark tests passed successfully!\n");
    return 0;
}
