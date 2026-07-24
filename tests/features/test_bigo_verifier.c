#include "bigo_verifier.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

// Sample O(N^2) dummy algorithm (Bubble Sort variant)
static void dummy_on2_alg(int* data, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n - 1; j++)
        {
            if (data[j] > data[j + 1])
            {
                int temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

// Sample O(N) dummy algorithm (Linear Scan)
static void dummy_on_alg(int* data, int n)
{
    long long sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum += data[i];
    }
    (void)sum;
}

void test_ratio_deduction(void)
{
    assert(deduce_bigo_from_ratio(1.0, 1000) == BIGO_O1);
    assert(deduce_bigo_from_ratio(1.3, 1000) == BIGO_OLOGN);
    assert(deduce_bigo_from_ratio(2.0, 1000) == BIGO_ON);
    assert(deduce_bigo_from_ratio(4.0, 1000) == BIGO_ON2);
    assert(deduce_bigo_from_ratio(8.0, 1000) == BIGO_ON3);
    printf("test_ratio_deduction passed!\n");
}

void test_bigo_profiler_execution(void)
{
    BigOReport report;
    bool success = run_bigo_profile("Dummy O(N^2) Alg", "O(N^2)", dummy_on2_alg, 200, 3, &report);
    assert(success == true);
    assert(report.sample_count == 3);
    assert(report.ratio_count == 2);
    assert(strcmp(report.algorithm_name, "Dummy O(N^2) Alg") == 0);

    print_bigo_report(&report);

    BigOReport report_on;
    success = run_bigo_profile("Dummy O(N) Alg", "O(N)", dummy_on_alg, 1000, 3, &report_on);
    assert(success == true);
    print_bigo_report(&report_on);

    printf("test_bigo_profiler_execution passed!\n");
}

int main(void)
{
    test_ratio_deduction();
    test_bigo_profiler_execution();
    return 0;
}
