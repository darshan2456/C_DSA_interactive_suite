#include "array.h"
#include "safe_input.h"
#include "sorting_visualizer.h"
#include <stdio.h>
#include <time.h>

void shell_sort(int arr[], int length_of_array);

/* Console interface for user to input array size and elements to sort */
void shell_sort_demo(void)
{
    int length_of_array;
    while (1)
    {
        printf("\n\nShell sort demo");
        int shell_sort_status = safe_input_int(&length_of_array,
                                               "\nenter the number of elements in the array "
                                               "(between 1 and 100), enter '-1' to exit:- ",
                                               1, 100);

        if (shell_sort_status == 0)
            continue;

        if (shell_sort_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting shell sort demo.\n");
            return;
        }

        int arr[length_of_array];

        // Gather all elements with safe input bounds
        for (int i = 0; i < length_of_array; i++)
        {
        retry:
            printf("\nenter the element number %d, (between 1 and 100), enter '-1' to exit:- ", i);
            int element_status = safe_input_int(&arr[i], NULL, 1, 100);

            if (element_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting shell sort demo.\n");
                return;
            }

            if (element_status == 0)
            {
                goto retry;
            }
        }

        printf("\nsee shell sort in action :-\n");
        shell_sort(arr, length_of_array);
    }
}

// note: the time measured by clock() function includes the time for shell sort computation and
// printing the array state after each iteration. the CPU time must not be treated as a measure of
// efficiency of the algorithm and is for demonstration only.
/* Performs shell sort using shell's original N/2, N/4... gap sequence */
void shell_sort(int arr[], int length_of_array)
{
    clock_t start_t, end_t;
    double total_t;

    start_t = clock();

    // Start with a large gap and reduce it by half each pass
    for (int gap = length_of_array / 2; gap > 0; gap /= 2)
    {
        // Run a gapped insertion sort for this gap size
        for (int i = gap; i < length_of_array; i++)
        {
            int temp = arr[i];
            int j;
            visualize_sort(arr, length_of_array, i, -1, -1,
                           "Shell Sort: Selecting element for insertion");
            // Shift earlier gap-sorted elements up until correct location is found
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap)
            {
                visualize_sort(arr, length_of_array, j, j - gap, -1,
                               "Shell Sort: Shifting elements");
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
            visualize_sort(arr, length_of_array, j, -1, -1, "Shell Sort: Inserting element");
        }

        printf("after gap of %d - ", gap);
        print_array(arr, length_of_array);
        printf("\n");
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    printf("\nfinal array sorted by shell sort - ");
    print_array(arr, length_of_array);
    printf("\nTotal CPU time taken:- %f seconds", total_t);
}
