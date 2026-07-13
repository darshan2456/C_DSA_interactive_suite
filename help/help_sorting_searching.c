#include "display_header.h"
#include "help.h"
#include "safe_input.h"
#include <stdio.h>

void help_sorting_searching_menu(void)
{
    while (1)
    {
        display_header("Help - Sorting & Searching");

        printf("Select a sub-topic:\n\n");
        printf("1. O(N^2) Sorting Algorithms\n");
        printf("2. Advanced O(N log N) Sorting Algorithms\n");
        printf("3. Searching Algorithms\n");
        int choice;
        int status =
            safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 1, 3);

        if (status == INPUT_EXIT_SIGNAL)
        {
            break;
        }

        if (status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                display_header("Help - O(N^2) Sorting Algorithms");
                printf("BUBBLE SORT:\n");
                printf("    Repeatedly steps through the list, compares adjacent elements, and "
                       "swaps them if in wrong order.\n\n");
                printf("SELECTION SORT:\n");
                printf("    Divides input into sorted and unsorted parts. Repeatedly finds minimum "
                       "element and moves it to sorted part.\n\n");
                printf("INSERTION SORT:\n");
                printf("    Builds final sorted array one item at a time by inserting elements "
                       "into their correct position.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 2:
                display_header("Help - Advanced Sorting Algorithms");
                printf("QUICK SORT:\n");
                printf("    Divide-and-conquer algorithm. Picks an element as pivot and partitions "
                       "the array around it.\n\n");
                printf("MERGE SORT:\n");
                printf("    Divide-and-conquer algorithm. Recursively splits array in halves, "
                       "sorts them, and merges them.\n\n");
                printf("HEAP SORT:\n");
                printf("    Comparison-based sorting technique based on Binary Heap data "
                       "structure.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;

            case 3:
                display_header("Help - Searching Algorithms");
                printf("LINEAR SEARCH:\n");
                printf("    Checks every element of the list sequentially until a match is found. "
                       "O(N) complexity.\n\n");
                printf("BINARY SEARCH:\n");
                printf("    Finds position of target value within a sorted array by repeatedly "
                       "halving the search interval. O(log N).\n\n");
                printf("JUMP SEARCH:\n");
                printf("    Searches in a sorted array by jumping ahead by fixed steps (usually "
                       "sqrt(N)).\n\n");
                printf("INTERPOLATION SEARCH:\n");
                printf("    Improved binary search for uniformly distributed sorted arrays. O(log "
                       "log N) average case.\n\n");
                printf("=================================================================\n");
                printf("Press [ENTER] to return...\n");
                printf("=================================================================\n");
                press_enter_to_continue();
                break;
        }
    }
}
