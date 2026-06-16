#include "advanced_sorting.h"
#include "safe_input.h"
#include <stdio.h>

void advanced_sorting_demo(void)
{

    int sorting_algo_status, sorting_algo_choice;
    while (1)
    {

        printf("\nAdvanced sorting algorithms demo\n");

        sorting_algo_status =
            safe_input_int(&sorting_algo_choice,
                           "enter '1' for quick sort, '2' for merge sort, '3' "
                           "for heap sort, '4' for radix sort, '5' for bucket sort '-1' to exit:- ",
                           1, 5);

        if (sorting_algo_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting advanced sorting algorithms demo\n");
            return;
        }

        if (sorting_algo_status == 0)
        {
            continue;
        }

        if (sorting_algo_choice == 1)
        {
            quicksort_demo();
        }

        else if (sorting_algo_choice == 2)
        {
            merge_sort_demo();
        }

        else if (sorting_algo_choice == 3)
        {
            heap_sort_demo();
        }

        else if (sorting_algo_choice == 4)
        {
            radix_sort_demo();
        }

        else if (sorting_algo_choice == 5)
        {
            bucket_sort_demo();
        }
    }
}
