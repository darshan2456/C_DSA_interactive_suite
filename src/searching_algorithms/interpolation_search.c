#include "safe_input.h"
#include "searching_algorithms.h"
#include "sorting_algorithms_n2.h"
#include <stdio.h>
#include <time.h>

/*
 * Interpolation Search
 *
 * Works on sorted arrays.
 *
 * Instead of always checking the middle element like Binary Search,
 * it estimates the likely position of the target based on its value.
 *
 * Best Case: O(1)
 * Average Case: O(log log n)
 * Worst Case: O(n)
 */

int interpolation_search(int arr[], int target, int length_of_array)
{
    int low = 0;
    int high = length_of_array - 1;

    while (low <= high && target >= arr[low] && target <= arr[high])
    {
        if (low == high)
        {
            return (arr[low] == target) ? low : -1;
        }

        if (arr[low] == arr[high])
        {
            if (arr[low] == target)
                return low;
            return -1;
        }

        int pos =
            low + (int)(((double)(high - low) / (arr[high] - arr[low])) * (target - arr[low]));

        if (arr[pos] == target)
            return pos;

        if (arr[pos] < target)
            low = pos + 1;
        else
            high = pos - 1;
    }

    return -1;
}

void interpolation_search_demo(void)
{
    int length_of_array;

    while (1)
    {
        printf("\nInterpolation search demo :- \n");

        int length_status = safe_input_int(
            &length_of_array, "\nenter length of array, (between 1 and 100), enter '-1' to exit:- ",
            1, 100);

        if (length_status == 0)
            continue;

        if (length_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting interpolation search demo....\n");
            return;
        }

        int arr[length_of_array];

        for (int i = 0; i < length_of_array; i++)
        {
        retry_element:

            printf("\nenter element no %d, (between 1 and 100), enter '-1' to exit:- ", i);

            int element_status = safe_input_int(&arr[i], NULL, 1, 100);

            if (element_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting interpolation search demo....\n");
                return;
            }

            if (element_status == 0)
                goto retry_element;
        }

        int target;

    target_retry:

        printf(
            "\nEnter target which you want to search, (between 1 and 100), enter '-1' to exit:- ");

        int target_status = safe_input_int(&target, NULL, 1, 100);

        if (target_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting interpolation search demo....\n");
            return;
        }

        if (target_status == 0)
            goto target_retry;

        selection_sort(arr, length_of_array);

        clock_t start_t = clock();

        int result = interpolation_search(arr, target, length_of_array);

        clock_t end_t = clock();

        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("\nelement found at index %d.", result);
        if (result == -1)
            printf("\nelement not found in the given array");

        printf("\ntotal CPU time taken for interpolation search:- %f seconds", total_t);
        printf("\n(most probably execution time would be lesser than clock resolution, resulting "
               "in 0.00)");
    }
}
