#include "benchmark.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_flow_benchmark_run(void)
{
    remove("benchmarks/flow_sparse.csv");
    remove("benchmarks/flow_dense.csv");

    run_flow_benchmark(20);

    FILE* file_sparse = fopen("benchmarks/flow_sparse.csv", "r");
    assert(file_sparse != NULL);
    char line[256];
    int row_count_sparse = 0;
    char* res = fgets(line, sizeof(line), file_sparse);
    assert(res != NULL);
    while (fgets(line, sizeof(line), file_sparse))
    {
        row_count_sparse++;
    }
    fclose(file_sparse);
    assert(row_count_sparse == 3);

    FILE* file_dense = fopen("benchmarks/flow_dense.csv", "r");
    assert(file_dense != NULL);
    int row_count_dense = 0;
    res = fgets(line, sizeof(line), file_dense);
    assert(res != NULL);
    while (fgets(line, sizeof(line), file_dense))
    {
        row_count_dense++;
    }
    fclose(file_dense);
    assert(row_count_dense == 3);

    remove("benchmarks/flow_sparse.csv");
    remove("benchmarks/flow_dense.csv");
    printf("test_flow_benchmark_run passed successfully.\n");
}

int main(void)
{
    test_flow_benchmark_run();
    printf("All flow benchmark tests passed successfully!\n");
    return 0;
}
