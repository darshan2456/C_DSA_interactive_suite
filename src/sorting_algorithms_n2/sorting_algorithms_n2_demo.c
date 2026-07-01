#include "display_header.h"
#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include <stdio.h>

void sorting_algorithms_n2_demo(void)
{
    int sorting_algo_status, sorting_algo_choice;
    while (1)
    {
        display_header("Sorting Algorithms (n^2)");

        sorting_algo_status = safe_input_int(&sorting_algo_choice,
                                             "\nenter 1 for bubble sort"
                                             "\nenter 2 for insertion sort"
                                             "\nenter 3 for selection sort"
                                             "\nenter 4 for shell sort"
                                             "\nenter choice : ",
                                             1, 4);

        if (sorting_algo_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting sorting_algorithms_demo.....\n");
            return;
        }

        if (sorting_algo_status == 0)
            continue;

        switch (sorting_algo_choice)
        {
            case 1:
                display_header("Bubble Sort");
                bubble_sort_optimized_demo();
                break;
            case 2:
                display_header("Insertion Sort");
                insertion_sort_demo();
                break;
            case 3:
                display_header("Selection Sort");
                selection_sort_demo();
                break;
            case 4:
                display_header("Shell Sort");
                shell_sort_demo();
                break;
        }
    }
}