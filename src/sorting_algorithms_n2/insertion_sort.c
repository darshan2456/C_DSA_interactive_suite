#include "array.h"
#include "safe_input.h"
#include "sorting_visualizer.h"
#include "step_debugger.h"
#include "telemetry.h"
#include <stdio.h>
#include <time.h>

void insertion_sort(int arr[], int length_of_array);

// note: the time measured by clock() function includes the time for insertion sort computation and
// printing the array state after each iteration. the CPU time must not be treated as a measure of
// efficiency of the algorithm and is for demonstration only.
void insertion_sort(int arr[], int length_of_array)
{
    telemetry_init("insertion_sort");

    clock_t start_t, end_t;
    double total_t;

    start_t = clock(); // time recorded at the start of algorithm

    for (int i = 1; i < length_of_array; i++)
    {
        int key = arr[i];
        int j = i - 1;
        char msg[128];
        snprintf(msg, sizeof(msg), "Insertion Sort: Selecting key at index %d (%d)", i, key);
        algorithm_step_hook(msg);
        visualize_sort(arr, length_of_array, i, -1, -1, "Insertion Sort: Selecting key");
        while (j >= 0 && arr[j] > key)
        {
            snprintf(msg, sizeof(msg), "Insertion Sort: Shifting index %d (%d) to %d", j, arr[j],
                     j + 1);
            algorithm_step_hook(msg);
            visualize_sort(arr, length_of_array, j, j + 1, -1, "Insertion Sort: Shifting element");
            arr[j + 1] = arr[j];
            j--;
        }
        snprintf(msg, sizeof(msg), "Insertion Sort: Inserting key (%d) at index %d", key, j + 1);
        algorithm_step_hook(msg);
        arr[j + 1] = key;
        visualize_sort(arr, length_of_array, j + 1, -1, -1, "Insertion Sort: Inserting key");

        printf("after iteration no %d - ", i);
        print_array(arr, length_of_array);
        printf("\n");
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    printf("\nfinal array sorted by insertion sort - ");
    print_array(arr, length_of_array);
    printf("\nTotal CPU time taken:- %f seconds", total_t);
    telemetry_close();
}