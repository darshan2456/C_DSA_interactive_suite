#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include <stdio.h>
#include <time.h>

int binary_search(int arr[], int target, int length_of_array);

void binary_search_demo(void)
{

    while (1)
    {
        clock_t start_t, end_t;
        double total_t;
        int length_of_array;
        int target;
        int binary_search_status;

        printf("\nBinary search demo :- \n");
        binary_search_status = safe_input_int(
            &length_of_array, "\nenter length of array, (between 1 and 100), enter '-1' to exit:- ",
            1, 100);

        if (binary_search_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting binary search demo....\n");
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
                printf("\nExiting linear search demo....\n");
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
                printf("\nExiting binary search demo....\n");
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
        int res = binary_search(arr, target, length_of_array);
        printf("\nelement found at index %d.", res);
        if (res == -1)
            printf("\nelement not found in the given array");
        end_t = clock();
        total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
        printf("\ntotal CPU time taken for binary search:- %f seconds", total_t);
        printf("\n(most probably execution time would be lesser than clock resolution, resulting "
               "in 0.00)");
    }
}

int binary_search(int arr[], int target, int length_of_array)
{
    int low = 0;
    int high = length_of_array - 1;
    int mid = 0;

    if (length_of_array == 1)
    {
        if (arr[0] == target)
        {
            return 0;
        }
        return -1;
    }

    while (low <= high)
    { // main loop which performs the binary search algorithm
        mid = low + (high - low) / 2;
        // printf("DEBUG: low=%d, mid=%d, high=%d, arr[mid]=%d\n", low, mid, high, arr[mid]);
        if (arr[mid] == target)
        {
            return mid;
        }
        else if (arr[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return -1;
}