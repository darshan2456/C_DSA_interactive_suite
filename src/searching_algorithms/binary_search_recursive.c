#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include <stdio.h>
#include <time.h>

int binary_search_recursive(int arr[], int target, int low, int high);

void binary_search_recursive_demo(void)
{

    while (1)
    {
        clock_t start_t, end_t;
        double total_t;
        int length_of_array;
        int target;
        int binary_search_status;

        printf("\nRecursive binary search demo :- \n");
        binary_search_status = safe_input_int(
            &length_of_array, "\nenter length of array, (between 1 and 100), enter '-1' to exit:- ",
            1, 100);

        if (binary_search_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting recursive binary search demo....\n");
            return;
        }

        if (binary_search_status == 0)
        {
            continue;
        }

        int arr[length_of_array];

        int element_status;

        for (int i = 0; i < length_of_array; i++)
        {

        retry_element:
            printf("\nenter element no %d, (between 1 and 100), enter '-1' to exit:- ", i);
            element_status = safe_input_int(&arr[i], NULL, 1, 100);

            if (element_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting recursive binary search demo....\n");
                return;
            }

            if (element_status == 0)
            {
                goto retry_element;
            }
        }

        int target_status;

        while (1)
        { // loop to validate target value insertion by user

            target_status = safe_input_int(&target,
                                           "\nEnter target which you want to search, (between 1 "
                                           "and 100), enter '-1' to exit:- ",
                                           1, 100);

            if (target_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting recursive binary search demo....\n");
                return;
            }

            if (target_status == 0)
            {
                continue;
            }

            break;
        }

        selection_sort(arr, length_of_array);

        start_t = clock();
        int res = binary_search_recursive(arr, target, 0, length_of_array - 1);
        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("\nelement found at index %d.", res);
        if (res == -1)
            printf("\nelement not found in the given array");

        printf("\ntotal CPU time taken for recursive binary search:- %f seconds", total_t);
        printf("\n(most probably execution time would be lesser than clock resolution, resulting "
               "in 0.00)");
    }
}

// recursively searches arr[low..high] for target, returning its index or -1 if absent.
// low and high carry the shrinking search window, so the demo starts it with 0 and length - 1.
int binary_search_recursive(int arr[], int target, int low, int high)
{
    if (low > high) // base case: empty window, target is not present
    {
        return -1;
    }

    int mid = low + (high - low) / 2;

    if (arr[mid] == target)
    {
        return mid;
    }
    else if (arr[mid] < target)
    {
        // target is greater, search the right half
        return binary_search_recursive(arr, target, mid + 1, high);
    }
    else
    {
        // target is smaller, search the left half
        return binary_search_recursive(arr, target, low, mid - 1);
    }
}
