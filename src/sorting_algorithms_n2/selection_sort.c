#include "array.h"
#include "safe_input.h"
#include "sorting_visualizer.h"
#include "step_debugger.h"
#include <stdio.h>
#include <time.h>

void selection_sort(int arr[], int length_of_array);

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
            char msg[128];
            snprintf(msg, sizeof(msg),
                     "Selection Sort: Comparing index %d (%d) and min_index %d (%d)", j, arr[j],
                     min_index, arr[min_index]);
            algorithm_step_hook(msg);
            visualize_sort(arr, length_of_array, j, min_index, i,
                           "Selection Sort: Comparing elements");
            if (arr[j] < arr[min_index])
            {
                min_index = j;
            }
        }

        if (i != min_index)
        {
            char msg[128];
            snprintf(msg, sizeof(msg), "Selection Sort: Swapping index %d and %d", i, min_index);
            algorithm_step_hook(msg);
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