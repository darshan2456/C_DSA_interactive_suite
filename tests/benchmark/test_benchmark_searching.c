#include "benchmark.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_searching_benchmark_run(void)
{
    // Clean up old files
    remove("benchmarks/searching.csv");

    // Run benchmark with size N = 100
    run_searching_benchmark(100);

    // Verify CSV output exists
    FILE* file = fopen("benchmarks/searching.csv", "r");
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

    // There should be exactly 5 data rows (one for each searching algorithm)
    assert(row_count == 5);

    // Clean up
    remove("benchmarks/searching.csv");
    printf("test_searching_benchmark_run passed successfully.\n");
}

int main(void)
{
    test_searching_benchmark_run();
    printf("All searching benchmark tests passed successfully!\n");
    return 0;
}
