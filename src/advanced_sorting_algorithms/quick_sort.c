#include "safe_input.h"
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

static int partition(int arr[], int low, int high, int total_len)
{
    int pivot = arr[low];
    int left = low + 1;
    int right = high;

    visualize_sort(arr, total_len, -1, -1, low, "Quick Sort: Partitioning (Pivot selected)");

    while (1)
    {
        while (left <= right && arr[left] <= pivot)
        {
            visualize_sort(arr, total_len, left, -1, low, "Quick Sort: Scanning left");
            left++;
        }
        while (arr[right] > pivot)
        {
            visualize_sort(arr, total_len, right, -1, low, "Quick Sort: Scanning right");
            right--;
        }
        if (left >= right)
            break;

        visualize_sort(arr, total_len, left, right, low,
                       "Quick Sort: Swapping out-of-order elements");
        swap(&arr[left], &arr[right]);
    }
    visualize_sort(arr, total_len, low, right, -1, "Quick Sort: Placing pivot in correct position");
    swap(&arr[low], &arr[right]);

    return right;
}

static void quicksort_recursive(int arr[], int low, int high, int total_len)
{
    if (low < high)
    {
        int p = partition(arr, low, high, total_len);
        quicksort_recursive(arr, low, p - 1, total_len);
        quicksort_recursive(arr, p + 1, high, total_len);
    }
}

void quicksort(int arr[], int low, int high)
{
    telemetry_init("quick_sort");
    int total_len = (high >= low) ? (high + 1) : 0;
    quicksort_recursive(arr, low, high, total_len);
    telemetry_close();
}
