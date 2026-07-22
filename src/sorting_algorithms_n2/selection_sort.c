#include "array.h"
#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include "sorting_telemetry.h"
#include "sorting_visualizer.h"
#include "step_debugger.h"
#include "telemetry.h"
#include <stdio.h>
#include <time.h>

void selection_sort_with_telemetry(int arr[], int length_of_array, SortingTelemetry* telemetry)
{
    if (telemetry)
    {
        sorting_telemetry_init(telemetry, "Selection Sort");
        sorting_telemetry_start(telemetry);
    }
    telemetry_init("selection_sort");

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
            sorting_telemetry_add_comparison(telemetry, 1);

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
            sorting_telemetry_add_swap(telemetry, 1);
            visualize_sort(arr, length_of_array, i, min_index, -1,
                           "Selection Sort: Swapping elements");
        }
    }

    telemetry_close();
    if (telemetry)
    {
        sorting_telemetry_stop(telemetry);
    }
}

void selection_sort(int arr[], int length_of_array)
{
    selection_sort_with_telemetry(arr, length_of_array, NULL);
}