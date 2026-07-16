#include "safe_input.h"
#include "searching_algorithms.h"
#include "sorting_algorithms_n2.h"
#include <stdio.h>
#include <time.h>

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
    }
}
