#include "benchmark.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_dp_benchmark_run(void)
{
    // Clean up old files
    remove("benchmarks/dp.csv");

    // Run benchmark with size N = 10
    run_dp_benchmark(10);

    // Verify CSV output exists
    FILE* file = fopen("benchmarks/dp.csv", "r");
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
    remove("benchmarks/dp.csv");
    printf("test_dp_benchmark_run passed successfully.\n");
}

int main(void)
{
    test_dp_benchmark_run();
    printf("All Dynamic Programming benchmark tests passed successfully!\n");
    return 0;
}
