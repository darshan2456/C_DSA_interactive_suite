#include "advanced_sorting.h"
#include "safe_input.h"
#include "sorting_visualizer.h"
#include "telemetry.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
