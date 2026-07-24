#include "bigo_verifier.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_ratio_deduction(void)
{
    assert(deduce_bigo_from_ratio(1.0, 1000) == BIGO_O1);
    assert(deduce_bigo_from_ratio(1.3, 1000) == BIGO_OLOGN);
    assert(deduce_bigo_from_ratio(2.0, 1000) == BIGO_ON);
    assert(deduce_bigo_from_ratio(4.0, 1000) == BIGO_ON2);
    assert(deduce_bigo_from_ratio(8.0, 1000) == BIGO_ON3);
    printf("test_ratio_deduction passed!\n");
}

void test_sorting_searching_profilers(void)
{
    BigOReport report;

    assert(profile_bubble_sort_bigo(100, &report) == true);
    assert(report.sample_count == 4);
    assert(strcmp(report.algorithm_name, "Bubble Sort") == 0);
    print_bigo_report(&report);

    assert(profile_quick_sort_bigo(1000, &report) == true);
    assert(strcmp(report.algorithm_name, "Quick Sort") == 0);
    print_bigo_report(&report);

    assert(profile_linear_search_bigo(10000, &report) == true);
    assert(strcmp(report.algorithm_name, "Linear Search") == 0);
    print_bigo_report(&report);

    printf("test_sorting_searching_profilers passed!\n");
}

void test_advanced_profilers(void)
{
    BigOReport report;

    assert(profile_bfs_graph_bigo(1000, &report) == true);
    assert(strcmp(report.algorithm_name, "BFS Graph Traversal") == 0);
    print_bigo_report(&report);

    assert(profile_bst_operations_bigo(500, &report) == true);
    assert(strcmp(report.algorithm_name, "BST Node Insertion") == 0);
    print_bigo_report(&report);

    assert(profile_dp_fibonacci_bigo(10000, &report) == true);
    assert(strcmp(report.algorithm_name, "DP Fibonacci") == 0);
    print_bigo_report(&report);

    printf("test_advanced_profilers passed!\n");
}

int main(void)
{
    test_ratio_deduction();
    test_sorting_searching_profilers();
    test_advanced_profilers();
    return 0;
}
