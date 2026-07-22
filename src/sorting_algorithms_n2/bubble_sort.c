#include "array.h"
#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include "sorting_telemetry.h"
#include "sorting_visualizer.h"
#include "step_debugger.h"
#include "telemetry.h"
#include <stdio.h>
#include <time.h>

void bubble_sort_optimized_with_telemetry(int arr[], int length_of_array,
                                          SortingTelemetry* telemetry)
{
    if (telemetry)
    {
        sorting_telemetry_init(telemetry, "Bubble Sort");
        sorting_telemetry_start(telemetry);
    }
    telemetry_init("bubble_sort");

    for (int i = 0; i < length_of_array - 1; i++)
    {
        int swapped = 0;
        sorting_telemetry_add_pass(telemetry, 1);

        for (int j = 0; j < length_of_array - i - 1; j++)
        {
            char msg[128];
            snprintf(msg, sizeof(msg), "Bubble Sort: Comparing index %d (%d) and %d (%d)", j,
                     arr[j], j + 1, arr[j + 1]);
            algorithm_step_hook(msg);
            visualize_sort(arr, length_of_array, j, j + 1, -1, "Bubble Sort: Comparing elements");
            sorting_telemetry_add_comparison(telemetry, 1);

            if (arr[j] > arr[j + 1])
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = 1;
                sorting_telemetry_add_swap(telemetry, 1);

                snprintf(msg, sizeof(msg), "Bubble Sort: Swapped index %d and %d", j, j + 1);
                algorithm_step_hook(msg);
                visualize_sort(arr, length_of_array, j, j + 1, -1,
                               "Bubble Sort: Swapping elements");
            }
        }

        if (swapped == 0)
            break;
    }

    telemetry_close();
    if (telemetry)
    {
        sorting_telemetry_stop(telemetry);
    }
}

void bubble_sort_optimized(int arr[], int length_of_array)
{
    bubble_sort_optimized_with_telemetry(arr, length_of_array, NULL);
}