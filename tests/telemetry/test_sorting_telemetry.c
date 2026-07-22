#include "advanced_sorting.h"
#include "sorting_telemetry.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

void test_sorting_telemetry_basic(void)
{
    SortingTelemetry t;
    sorting_telemetry_init(&t, "TestSort");
    assert(t.comparisons == 0);
    assert(t.swaps == 0);
    assert(t.copies == 0);
    assert(t.max_recursion_depth == 0);
    assert(t.current_recursion_depth == 0);

    sorting_telemetry_start(&t);
    usleep(1000); // 1 ms delay
    sorting_telemetry_stop(&t);

    assert(t.elapsed_microseconds > 0.0);

    sorting_telemetry_add_comparison(&t, 10);
    sorting_telemetry_add_swap(&t, 5);
    sorting_telemetry_add_copy(&t, 8);
    sorting_telemetry_add_pass(&t, 3);

    assert(t.comparisons == 10);
    assert(t.swaps == 5);
    assert(t.copies == 8);
    assert(t.pass_count == 3);

    sorting_telemetry_enter_recursion(&t);
    sorting_telemetry_enter_recursion(&t);
    assert(t.current_recursion_depth == 2);
    assert(t.max_recursion_depth == 2);

    sorting_telemetry_exit_recursion(&t);
    assert(t.current_recursion_depth == 1);
    assert(t.max_recursion_depth == 2);

    sorting_telemetry_reset(&t);
    assert(t.comparisons == 0);
    assert(t.swaps == 0);

    printf("test_sorting_telemetry_basic passed successfully!\n");
}

void test_quicksort_telemetry(void)
{
    int arr[] = {5, 2, 9, 1, 7, 6, 3};
    int n = 7;
    SortingTelemetry t;

    quicksort_with_telemetry(arr, 0, n - 1, &t);

    assert(t.comparisons > 0);
    assert(t.swaps > 0);
    assert(t.max_recursion_depth > 0);
    assert(t.elapsed_microseconds >= 0.0);

    for (int i = 0; i < n - 1; i++)
    {
        assert(arr[i] <= arr[i + 1]);
    }

    printf("test_quicksort_telemetry passed successfully!\n");
}

void test_mergesort_telemetry(void)
{
    int arr[] = {8, 4, 12, 2, 10, 6, 14};
    int n = 7;
    SortingTelemetry t;

    merge_sort_with_telemetry(arr, n, &t);

    assert(t.comparisons > 0);
    assert(t.copies > 0);
    assert(t.max_recursion_depth > 0);
    assert(t.elapsed_microseconds >= 0.0);

    for (int i = 0; i < n - 1; i++)
    {
        assert(arr[i] <= arr[i + 1]);
    }

    printf("test_mergesort_telemetry passed successfully!\n");
}

void test_heapsort_telemetry(void)
{
    int arr[] = {15, 3, 9, 1, 11, 7, 5};
    int n = 7;
    SortingTelemetry t;

    heap_sort_with_telemetry(arr, n, &t);

    assert(t.comparisons > 0);
    assert(t.swaps > 0 || t.copies > 0);
    assert(t.elapsed_microseconds >= 0.0);

    for (int i = 0; i < n - 1; i++)
    {
        assert(arr[i] <= arr[i + 1]);
    }

    printf("test_heapsort_telemetry passed successfully!\n");
}

void test_radixsort_telemetry(void)
{
    int arr[] = {170, 45, 75, 90, 802, 24, 2, 66};
    int n = 8;
    SortingTelemetry t;

    radix_sort_with_telemetry(arr, n, &t);

    assert(t.comparisons > 0);
    assert(t.copies > 0);
    assert(t.pass_count > 0);
    assert(t.elapsed_microseconds >= 0.0);

    for (int i = 0; i < n - 1; i++)
    {
        assert(arr[i] <= arr[i + 1]);
    }

    printf("test_radixsort_telemetry passed successfully!\n");
}

int main(void)
{
    test_sorting_telemetry_basic();
    test_quicksort_telemetry();
    test_mergesort_telemetry();
    test_heapsort_telemetry();
    test_radixsort_telemetry();
    return 0;
}
