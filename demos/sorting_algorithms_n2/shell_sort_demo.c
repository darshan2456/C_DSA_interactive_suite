#include "array.h"
#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include "sorting_visualizer.h"
#include "step_debugger.h"
#include <stdio.h>
#include <time.h>

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
