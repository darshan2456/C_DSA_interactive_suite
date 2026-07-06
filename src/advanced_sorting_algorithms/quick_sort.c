#include "safe_input.h"
#include "sorting_visualizer.h"
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
    int total_len = (high >= low) ? (high + 1) : 0;
    quicksort_recursive(arr, low, high, total_len);
}

void quicksort_demo(void)
{
    int length_of_array;
    while (1)
    {
        printf("\n\nQuicksort demo");
        int quicksort_status = safe_input_int(&length_of_array,
                                              "\nenter the number of elements in the array "
                                              "(between 1 and 100), enter '-1' to exit:- ",
                                              1, 100);

        if (quicksort_status == 0)
            continue; // retry on failure

        if (quicksort_status == INPUT_EXIT_SIGNAL)
        { // exit condition, user enters '-1'
            printf("\nExiting quicksort demo....\n");
            return;
        }

        int arr[length_of_array];

        int i = length_of_array - 1;

        do
        {

        retry:
            printf("\nenter the element number %d, (between 1 and 100), enter '-1' to exit:- ", i);
            int element_status = safe_input_int(&arr[i], NULL, 1, 100);

            if (element_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting quicksort demo.....\n\n");
                return;
            }

            if (element_status == 0)
            {
                goto retry;
            }

            i--;
        } while (i >= 0);

        clock_t start_t = clock();
        quicksort(arr, 0, length_of_array - 1);
        clock_t end_t = clock();
        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("\nThe array sorted by quicksort is:- ");
        for (int i = 0; i < length_of_array; i++)
        {
            printf("%d", arr[i]);
            if (i < length_of_array - 1)
                printf(",");
        }
        printf("\nTotal CPU time taken:- %f seconds", total_t);
    }
}
