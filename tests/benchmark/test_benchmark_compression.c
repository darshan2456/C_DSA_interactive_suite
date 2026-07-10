#include "benchmark.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_compression_benchmark_run(void)
{
    remove("benchmarks/compression.csv");

    run_compression_benchmark(100);

    FILE* file = fopen("benchmarks/compression.csv", "r");
    assert(file != NULL);

    char line[256];
    int row_count = 0;

    char* res = fgets(line, sizeof(line), file);
    assert(res != NULL);
    assert(
        strstr(line, "Algorithm,Input Size,Execution Time (Seconds),Peak Memory (KB),Timestamp") !=
        NULL);

    while (fgets(line, sizeof(line), file))
    {
        row_count++;
    }
    fclose(file);

    // 4 algorithms * 3 input types = 12 data rows
    assert(row_count == 12);

    remove("benchmarks/compression.csv");
    printf("test_compression_benchmark_run passed successfully.\n");
}

int main(void)
{
    test_compression_benchmark_run();
    printf("All compression benchmark tests passed successfully!\n");
    return 0;
}
