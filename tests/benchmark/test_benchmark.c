#include "benchmark.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_timing(void)
{
    double start = benchmark_get_time();
    // Busy wait for a short moment
    volatile int dummy = 0;
    for (int i = 0; i < 10000000; i++)
    {
        dummy++;
    }
    double end = benchmark_get_time();
    assert(end > start);
    printf("test_timing passed. Start: %f, End: %f, Diff: %f\n", start, end, end - start);
}

void test_memory(void)
{
    size_t mem_before = benchmark_get_peak_memory();
    (void)mem_before; // Suppress unused warning

    // Allocate 5MB to show memory activity
    size_t size = 5 * 1024 * 1024;
    char* buf = malloc(size);
    assert(buf != NULL);
    memset(buf, 0, size); // Write to make it resident

    size_t mem_after = benchmark_get_peak_memory();
    free(buf);

    printf("test_memory passed. Peak RAM: %zu KB\n", mem_after);
}

void test_csv_export(void)
{
    const char* category = "test_category";
    const char* algo = "Test Algo";
    int size = 42;
    double time_val = 0.123456;
    size_t mem = 2048;

    // Remove existing file if any
    remove("benchmarks/test_category.csv");

    int status = benchmark_export_csv(category, algo, size, time_val, mem);
    assert(status == 0);

    FILE* file = fopen("benchmarks/test_category.csv", "r");
    assert(file != NULL);

    char line[256];
    // Read header
    char* res = fgets(line, sizeof(line), file);
    assert(res != NULL);
    assert(
        strstr(line, "Algorithm,Input Size,Execution Time (Seconds),Peak Memory (KB),Timestamp") !=
        NULL);

    // Read data row
    res = fgets(line, sizeof(line), file);
    assert(res != NULL);
    assert(strstr(line, "Test Algo,42,0.123456,2048") != NULL);

    fclose(file);

    // Clean up test file
    remove("benchmarks/test_category.csv");
    printf("test_csv_export passed.\n");
}

int main(void)
{
    test_timing();
    test_memory();
    test_csv_export();
    printf("All core benchmark tests passed successfully!\n");
    return 0;
}
