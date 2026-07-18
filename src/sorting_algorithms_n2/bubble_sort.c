#include "array.h"
#include "safe_input.h"
#include "sorting_visualizer.h"
#include "step_debugger.h"
#include "telemetry.h"
#include <stdio.h>
#include <time.h>

void bubble_sort_optimized(int arr[], int length_of_array);

// note: the time measured by clock() function includes the time for bubble sort computation and
// printing the array state after each iteration. the CPU time must not be treated as a measure of
// efficiency of the algorithm and is for demonstration only.
void bubble_sort_optimized(int arr[], int length_of_array)
{
    telemetry_init("bubble_sort");

    clock_t start_t, end_t;
    double total_t;

    start_t = clock(); // time recorded at the start of the algorithm

    for (int i = 0; i < length_of_array - 1; i++)
    {
        int swapped = 0;

        for (int j = 0; j < length_of_array - i - 1; j++)
        { // Swapping happens here
            char msg[128];
            snprintf(msg, sizeof(msg), "Bubble Sort: Comparing index %d (%d) and %d (%d)", j,
                     arr[j], j + 1, arr[j + 1]);
            algorithm_step_hook(msg);
            visualize_sort(arr, length_of_array, j, j + 1, -1, "Bubble Sort: Comparing elements");
            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
                snprintf(msg, sizeof(msg), "Bubble Sort: Swapped index %d and %d", j, j + 1);
                algorithm_step_hook(msg);
                visualize_sort(arr, length_of_array, j, j + 1, -1,
                               "Bubble Sort: Swapping elements");
            }
        }

        if (swapped == 0)
            break;
        printf("after iteration no %d - ", i + 1);
        print_array(arr, length_of_array);
        printf("\n");
    }

    end_t = clock(); // time recorded at the end of the algorithm

    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    printf("\nfinal array sorted by bubble sort is - ");
    print_array(arr, length_of_array);
    printf("\nTotal CPU time taken:- %f seconds", total_t);
    telemetry_close();
}