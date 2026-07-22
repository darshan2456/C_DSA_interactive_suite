#include "advanced_sorting.h"
#include "safe_input.h"
#include "sorting_telemetry.h"
#include "sorting_visualizer.h"
#include "telemetry.h"
#include <stdio.h>
#include <time.h>

static void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static int partition(int arr[], int low, int high, int total_len, SortingTelemetry* telemetry)
{
    int pivot = arr[low];
    int left = low + 1;
    int right = high;

    visualize_sort(arr, total_len, -1, -1, low, "Quick Sort: Partitioning (Pivot selected)");

    while (1)
    {
        while (left <= right)
        {
            sorting_telemetry_add_comparison(telemetry, 1);
            if (arr[left] > pivot)
                break;
            visualize_sort(arr, total_len, left, -1, low, "Quick Sort: Scanning left");
            left++;
        }
        while (right >= low + 1)
        {
            sorting_telemetry_add_comparison(telemetry, 1);
            if (arr[right] <= pivot)
                break;
            visualize_sort(arr, total_len, right, -1, low, "Quick Sort: Scanning right");
            right--;
        }
        if (left >= right)
            break;

        visualize_sort(arr, total_len, left, right, low,
                       "Quick Sort: Swapping out-of-order elements");
        swap(&arr[left], &arr[right]);
        sorting_telemetry_add_swap(telemetry, 1);
    }
    visualize_sort(arr, total_len, low, right, -1, "Quick Sort: Placing pivot in correct position");
    swap(&arr[low], &arr[right]);
    sorting_telemetry_add_swap(telemetry, 1);

    return right;
}

static void quicksort_recursive(int arr[], int low, int high, int total_len,
                                SortingTelemetry* telemetry)
{
    if (low < high)
    {
        sorting_telemetry_enter_recursion(telemetry);
        int p = partition(arr, low, high, total_len, telemetry);
        quicksort_recursive(arr, low, p - 1, total_len, telemetry);
        quicksort_recursive(arr, p + 1, high, total_len, telemetry);
        sorting_telemetry_exit_recursion(telemetry);
    }
}

void quicksort_with_telemetry(int arr[], int low, int high, SortingTelemetry* telemetry)
{
    if (telemetry)
    {
        sorting_telemetry_init(telemetry, "Quick Sort");
        sorting_telemetry_start(telemetry);
    }
    telemetry_init("quick_sort");
    int total_len = (high >= low) ? (high + 1) : 0;
    quicksort_recursive(arr, low, high, total_len, telemetry);
    telemetry_close();
    if (telemetry)
    {
        sorting_telemetry_stop(telemetry);
    }
}

void quicksort(int arr[], int low, int high)
{
    quicksort_with_telemetry(arr, low, high, NULL);
}
