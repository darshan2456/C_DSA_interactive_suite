#include "data_structures.h"
#include "safe_input.h"
#include "sorting_visualizer.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void selection_sort(int arr[], int length_of_array);

void selection_sort_demo(void)
{
    int length_of_array;
    while (1)
    {
        printf("\n\nSelection sort demo");
        int selection_sort_status = safe_input_int(&length_of_array,
                                                   "\nenter the number of elements in the array "
                                                   "(between 1 and 100), enter '-1' to exit:- ",
                                                   1, 100);

        if (selection_sort_status == 0)
            continue; // retry on failure

        if (selection_sort_status == INPUT_EXIT_SIGNAL)
        { // exit condition, user enters '-1'
            printf("\nExiting selection sort demo....\n");
            return;
        }
        int arr[length_of_array];

        for (int i = 0; i < length_of_array; i++)
        {

        retry:
            printf("\nenter element no %d, (between 1 and 100), enter '-1' to exit:- ", i);
            int element_status = safe_input_int(&arr[i], NULL, 1, 100);
            if (element_status == INPUT_EXIT_SIGNAL)
            { // exit condition, user entered '-1'
                printf("\nExiting selection sort demo.....\n");
                return;
            }

            if (element_status == 0)
            {
                goto retry;
            }
        }

        selection_sort(arr, length_of_array);
    }
}

void selection_sort(int arr[], int length_of_array)
{
    clock_t start_t, end_t;
    double total_t;

    start_t = clock();

    for (int i = 0; i < length_of_array - 1; i++)
    {
        int min_index = i;

        for (int j = i + 1; j < length_of_array; j++)
        {
            visualize_sort(arr, length_of_array, j, min_index, i,
                           "Selection Sort: Comparing elements");
            if (arr[j] < arr[min_index])
            {
                min_index = j;
            }
        }

        if (i != min_index)
        {
            int temp = arr[min_index];
            arr[min_index] = arr[i];
            arr[i] = temp;
            visualize_sort(arr, length_of_array, i, min_index, -1,
                           "Selection Sort: Swapping elements");
        }

        // printf("after iteration no %d - ", i + 1);
        // print_array(arr, length_of_array);
        // printf("\n");
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    printf("\nfinal array sorted by selection sort is:- ");
    print_array(arr, length_of_array);
    printf("\nTotal CPU time taken:- %f seconds", total_t);
}