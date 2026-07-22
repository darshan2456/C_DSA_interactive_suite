#include "array.h"
#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include "sorting_telemetry.h"
#include "sorting_visualizer.h"
#include "step_debugger.h"
#include "telemetry.h"
#include <stdio.h>
#include <time.h>

void insertion_sort_with_telemetry(int arr[], int length_of_array, SortingTelemetry* telemetry)
{
    if (telemetry)
    {
        sorting_telemetry_init(telemetry, "Insertion Sort");
        sorting_telemetry_start(telemetry);
    }
    telemetry_init("insertion_sort");

    for (int i = 1; i < length_of_array; i++)
    {
        int key = arr[i];
        sorting_telemetry_add_copy(telemetry, 1);
        int j = i - 1;
        char msg[128];
        snprintf(msg, sizeof(msg), "Insertion Sort: Selecting key at index %d (%d)", i, key);
        algorithm_step_hook(msg);
        visualize_sort(arr, length_of_array, i, -1, -1, "Insertion Sort: Selecting key");

        while (j >= 0)
        {
            sorting_telemetry_add_comparison(telemetry, 1);
            if (arr[j] <= key)
                break;

            snprintf(msg, sizeof(msg), "Insertion Sort: Shifting index %d (%d) to %d", j, arr[j],
                     j + 1);
            algorithm_step_hook(msg);
            visualize_sort(arr, length_of_array, j, j + 1, -1, "Insertion Sort: Shifting element");
            arr[j + 1] = arr[j];
            sorting_telemetry_add_copy(telemetry, 1);
            j--;
        }
        snprintf(msg, sizeof(msg), "Insertion Sort: Inserting key (%d) at index %d", key, j + 1);
        algorithm_step_hook(msg);
        arr[j + 1] = key;
        sorting_telemetry_add_copy(telemetry, 1);
        visualize_sort(arr, length_of_array, j + 1, -1, -1, "Insertion Sort: Inserting key");
    }

    telemetry_close();
    if (telemetry)
    {
        sorting_telemetry_stop(telemetry);
    }
}

void insertion_sort(int arr[], int length_of_array)
{
    insertion_sort_with_telemetry(arr, length_of_array, NULL);
}