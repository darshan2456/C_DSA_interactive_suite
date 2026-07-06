#include "advanced_sorting.h"
#include "safe_input.h"
#include "sorting_visualizer.h"
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
    merge_recursion(arr, 0, n - 1, n);
}

void merge_sort_demo(void)
{
    int length_of_array;
    while (1)
    {
        printf("\n\nMerge sort demo");
        int quicksort_status = safe_input_int(&length_of_array,
                                              "\nenter the number of elements in the array "
                                              "(between 1 and 100), enter '-1' to exit:- ",
                                              1, 100);

        if (quicksort_status == 0)
            continue; // retry on failure

        if (quicksort_status == INPUT_EXIT_SIGNAL)
        { // exit condition, user enters '-1'
            printf("\nExiting merge sort demo....\n");
            return;
        }

        int arr[length_of_array];

        for (int i = 0; i < length_of_array; i++)
        {

        retry:
            printf("\nenter the element number %d, (between 1 and 100), enter '-1' to exit:- ", i);
            int element_status = safe_input_int(&arr[i], NULL, 1, 100);

            if (element_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting merge sort demo.....\n");
                return;
            }

            if (element_status == 0)
            {
                goto retry;
            }
        }

        clock_t start_t = clock();
        merge_sort(arr, length_of_array);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("\nThe array sorted by merge sort is:- ");
        for (int i = 0; i < length_of_array; i++)
        {
            printf("%d", arr[i]);
            if (i < length_of_array - 1)
                printf(",");
        }
        printf("\nTotal CPU time taken:- %f seconds", total_t);
    }
}
