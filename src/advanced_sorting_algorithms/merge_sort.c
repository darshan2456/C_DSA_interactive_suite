#include "advanced_sorting.h"
#include "safe_input.h"
#include "sorting_visualizer.h"
#include "telemetry.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void merge(int arr[], int left, int mid, int right, int total_len)
{
    int left_length = mid - left + 1;
    int right_length = right - mid;

    int* left_array = malloc(sizeof(int) * left_length);
    int* right_array = malloc(sizeof(int) * right_length);

    if (left_array == NULL || right_array == NULL)
    {
        printf("\nMemory allocation failed during merge sort.\n");
        if (left_array)
            free(left_array);
        if (right_array)
            free(right_array);
        return;
    }

    for (int i = 0; i < left_length; i++) // copy left half into temp malloc'd array
    {
        left_array[i] = arr[left + i];
    }

    for (int j = 0; j < right_length; j++) // copy right half into temp malloc'd array
    {
        right_array[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < left_length && j < right_length) // merging of two sorted halves back into arr
    {
        visualize_sort(arr, total_len, left + i, mid + 1 + j, -1,
                       "Merge Sort: Comparing elements from subarrays");
        if (left_array[i] <= right_array[j])
        {
            arr[k++] = left_array[i++];
        }
        else
        {
            arr[k++] = right_array[j++];
        }
        visualize_sort(arr, total_len, k - 1, -1, -1, "Merge Sort: Writing element back");
    }

    while (i < left_length) // if right array was completed but left was not
    {
        arr[k++] = left_array[i++];
        visualize_sort(arr, total_len, k - 1, -1, -1, "Merge Sort: Writing remaining left element");
    }
    while (j < right_length) // if left array was completed but right was not
    {
        arr[k++] = right_array[j++];
        visualize_sort(arr, total_len, k - 1, -1, -1,
                       "Merge Sort: Writing remaining right element");
    }

    free(left_array);
    free(right_array);
}

static void merge_recursion(int arr[], int left, int right, int total_len)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        merge_recursion(arr, left, mid, total_len);
        merge_recursion(arr, mid + 1, right, total_len);

        merge(arr, left, mid, right, total_len);
    }
}

void merge_sort(int arr[], int n)
{
    if (n <= 1)
    {
        return;
    }
    telemetry_init("merge_sort");
    merge_recursion(arr, 0, n - 1, n);
    telemetry_close();
}
