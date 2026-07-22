#include "array.h"
#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include "sorting_telemetry.h"
#include "sorting_visualizer.h"
#include "telemetry.h"
#include <stdio.h>
#include <time.h>

void shell_sort_with_telemetry(int arr[], int length_of_array, SortingTelemetry* telemetry)
{
    if (telemetry)
    {
        sorting_telemetry_init(telemetry, "Shell Sort");
        sorting_telemetry_start(telemetry);
    }
    telemetry_init("shell_sort");

    for (int gap = length_of_array / 2; gap > 0; gap /= 2)
    {
        sorting_telemetry_add_pass(telemetry, 1);
        for (int i = gap; i < length_of_array; i++)
        {
            int temp = arr[i];
            sorting_telemetry_add_copy(telemetry, 1);
            int j;
            visualize_sort(arr, length_of_array, i, -1, -1,
                           "Shell Sort: Selecting element for insertion");
            for (j = i; j >= gap; j -= gap)
            {
                sorting_telemetry_add_comparison(telemetry, 1);
                if (arr[j - gap] <= temp)
                    break;
                visualize_sort(arr, length_of_array, j, j - gap, -1,
                               "Shell Sort: Shifting elements");
                arr[j] = arr[j - gap];
                sorting_telemetry_add_copy(telemetry, 1);
            }
            arr[j] = temp;
            sorting_telemetry_add_copy(telemetry, 1);
            visualize_sort(arr, length_of_array, j, -1, -1, "Shell Sort: Inserting element");
        }
    }

    telemetry_close();
    if (telemetry)
    {
        sorting_telemetry_stop(telemetry);
    }
}

void shell_sort(int arr[], int length_of_array)
{
    shell_sort_with_telemetry(arr, length_of_array, NULL);
}
