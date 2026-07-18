#include "advanced_sorting.h"
#include "sorting_algorithms_n2.h"
#include "telemetry.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void verify_telemetry_file(const char* expected_algo_name)
{
    FILE* fp = fopen("test_binaries/sorting_trace.json", "r");
    assert(fp != NULL);

    char buffer[2048];
    size_t bytes_read = fread(buffer, 1, sizeof(buffer) - 1, fp);
    buffer[bytes_read] = '\0';
    fclose(fp);

    // Verify it is a valid JSON array and has the correct algorithm name
    assert(buffer[0] == '[');
    assert(strstr(buffer, expected_algo_name) != NULL);
    assert(strstr(buffer, "\"step\":") != NULL);

    int len = strlen(buffer);
    while (len > 0 &&
           (buffer[len - 1] == '\n' || buffer[len - 1] == ' ' || buffer[len - 1] == '\r'))
    {
        len--;
    }
    assert(buffer[len - 1] == ']');

    // Clean up
    remove("test_binaries/sorting_trace.json");
}

static void test_bubble_sort_telemetry(void)
{
    set_telemetry_enabled(true);
    set_telemetry_filepath("test_binaries/sorting_trace.json");

    int arr[] = {4, 2, 1, 3};
    bubble_sort_optimized(arr, 4);

    verify_telemetry_file("bubble_sort");
}

static void test_insertion_sort_telemetry(void)
{
    set_telemetry_enabled(true);
    set_telemetry_filepath("test_binaries/sorting_trace.json");

    int arr[] = {4, 2, 1, 3};
    insertion_sort(arr, 4);

    verify_telemetry_file("insertion_sort");
}

static void test_selection_sort_telemetry(void)
{
    set_telemetry_enabled(true);
    set_telemetry_filepath("test_binaries/sorting_trace.json");

    int arr[] = {4, 2, 1, 3};
    selection_sort(arr, 4);

    verify_telemetry_file("selection_sort");
}

static void test_shell_sort_telemetry(void)
{
    set_telemetry_enabled(true);
    set_telemetry_filepath("test_binaries/sorting_trace.json");

    int arr[] = {4, 2, 1, 3};
    shell_sort(arr, 4);

    verify_telemetry_file("shell_sort");
}

static void test_quick_sort_telemetry(void)
{
    set_telemetry_enabled(true);
    set_telemetry_filepath("test_binaries/sorting_trace.json");

    int arr[] = {4, 2, 1, 3};
    quicksort(arr, 0, 3);

    verify_telemetry_file("quick_sort");
}

static void test_merge_sort_telemetry(void)
{
    set_telemetry_enabled(true);
    set_telemetry_filepath("test_binaries/sorting_trace.json");

    int arr[] = {4, 2, 1, 3};
    merge_sort(arr, 4);

    verify_telemetry_file("merge_sort");
}

static void test_heap_sort_telemetry(void)
{
    set_telemetry_enabled(true);
    set_telemetry_filepath("test_binaries/sorting_trace.json");

    int arr[] = {4, 2, 1, 3};
    heap_sort(arr, 4);

    verify_telemetry_file("heap_sort");
}

static void test_radix_sort_telemetry(void)
{
    set_telemetry_enabled(true);
    set_telemetry_filepath("test_binaries/sorting_trace.json");

    int arr[] = {4, 2, 1, 3};
    radix_sort(arr, 4);

    verify_telemetry_file("radix_sort");
}

static void test_bucket_sort_telemetry(void)
{
    set_telemetry_enabled(true);
    set_telemetry_filepath("test_binaries/sorting_trace.json");

    int arr[] = {4, 2, 1, 3};
    bucket_sort(arr, 4);

    verify_telemetry_file("bucket_sort");
}

int main(void)
{
    printf("Starting Sorting Telemetry integration tests...\n");
    test_bubble_sort_telemetry();
    test_insertion_sort_telemetry();
    test_selection_sort_telemetry();
    test_shell_sort_telemetry();
    test_quick_sort_telemetry();
    test_merge_sort_telemetry();
    test_heap_sort_telemetry();
    test_radix_sort_telemetry();
    test_bucket_sort_telemetry();
    printf("All Sorting Telemetry integration tests passed successfully!\n");
    return 0;
}
