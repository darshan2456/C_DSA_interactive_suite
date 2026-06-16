#include "history_logger.h"
#include "safe_input.h"
#include "searching_algorithms.h"
#include "sorting_algorithms_n2.h"
#include <stdio.h>
#include <time.h>

/*
 * Jump Search
 *
 * Works on sorted arrays.
 *
 * It jumps ahead by fixed-size steps (usually sqrt(n)) and then
 * performs a linear search backward or forward within the identified block.
 *
 * Best Case: O(1)
 * Average Case: O(sqrt(n))
 * Worst Case: O(sqrt(n))
 */

static int integer_sqrt(int n)
{
    if (n < 0)
    {
        return 0;
    }
    if (n == 0 || n == 1)
    {
        return n;
    }
    int start = 1, end = n, ans = 1;
    while (start <= end)
    {
        int mid = start + (end - start) / 2;
        if (mid <= n / mid)
        {
            start = mid + 1;
            ans = mid;
        }
        else
        {
            end = mid - 1;
        }
    }
    return ans;
}

int jump_search(int arr[], int target, int length_of_array)
{
    if (length_of_array <= 0)
    {
        return -1;
    }

    int step = integer_sqrt(length_of_array);
    int prev = 0;

    while (arr[prev + step - 1 < length_of_array ? prev + step - 1 : length_of_array - 1] < target)
    {
        prev += step;
        if (prev >= length_of_array)
        {
            return -1;
        }
    }

    int limit = prev + step < length_of_array ? prev + step : length_of_array;
    for (int i = prev; i < limit; i++)
    {
        if (arr[i] == target)
        {
            return i;
        }
    }

    return -1;
}

void jump_search_demo(void)
{
    int length_of_array;

    while (1)
    {
        printf("\nJump search demo :- \n");

        int length_status = safe_input_int(
            &length_of_array, "\nenter length of array, (between 1 and 100), enter '-1' to exit:- ",
            1, 100);

        if (length_status == 0)
        {
            continue;
        }

        if (length_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting jump search demo....\n");
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
                printf("\nExiting jump search demo....\n");
                return;
            }

            if (element_status == 0)
            {
                goto retry_element;
            }
        }

        int target;

    target_retry:

        printf(
            "\nEnter target which you want to search, (between 1 and 100), enter '-1' to exit:- ");

        int target_status = safe_input_int(&target, NULL, 1, 100);

        if (target_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting jump search demo....\n");
            return;
        }

        if (target_status == 0)
        {
            goto target_retry;
        }

        selection_sort(arr, length_of_array);

        clock_t start_t = clock();

        int result = jump_search(arr, target, length_of_array);

        clock_t end_t = clock();

        double total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

        printf("\nelement found at index %d.", result);
        if (result == -1)
        {
            printf("\nelement not found in the given array");
        }

        printf("\ntotal CPU time taken for jump search:- %f seconds", total_t);
        printf("\n(most probably execution time would be lesser than clock resolution, resulting "
               "in 0.00)");

        add_to_history("Jump Search", length_of_array, total_t);
    }
}
