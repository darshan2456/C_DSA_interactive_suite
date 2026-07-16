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
        printf("2. Advanced Sorting Algorithms (Quick, Merge, Heap, Bucket, Radix)\n");
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
                printf("SHELL SORT:\n");
                printf("    A generalization of Insertion Sort that allows the exchange of "
                       "far-apart\n");
                printf("    elements, reducing the array's inversion count step-by-step using a "
                       "gap sequence.\n\n");

                printf("RELATIONSHIPS & COMPARISONS:\n");
                printf("    • Bubble vs. Selection vs. Insertion:\n");
                printf("      - Bubble Sort is generally the least efficient due to excessive "
                       "swaps.\n");
                printf("      - Selection Sort minimizes write operations (ideal when memory "
                       "writes are costly).\n");
                printf("      - Insertion Sort is extremely fast for nearly-sorted or small "
                       "datasets.\n");
                printf("    • Insertion vs. Shell Sort:\n");
                printf("      - Shell Sort acts as a bridge between simple O(N^2) and fast O(N log "
                       "N) sorts.\n");
                printf("      - By doing long-distance swaps, Shell Sort reduces inversions "
                       "quickly, allowing\n");
                printf("        subsequent passes to run in near-linear time.\n\n");

                printf("WHEN TO USE WHAT & REAL-WORLD USE CASES:\n");
                printf("    • Use Insertion Sort for small arrays (typically N < 15) or "
                       "nearly-sorted lists\n");
                printf(
                    "      (often used as the base-case cutoff for Quick Sort and Merge Sort).\n");
                printf("    • Use Shell Sort in embedded systems with strict memory bounds where "
                       "recursive\n");
                printf("      stack overhead (of Quick Sort) or auxiliary memory (of Merge Sort) "
                       "is unacceptable.\n\n");

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
                printf("BUCKET SORT:\n");
                printf("    Distributes elements into buckets, sorts each bucket, and "
                       "concatenates them.\n");
                printf("    Average O(N + K) for uniformly distributed input.\n\n");
                printf("RADIX SORT:\n");
                printf("    Non-comparison sort that processes digits from least to most "
                       "significant.\n");
                printf("    O(D * (N + K)) where D is the number of digits.\n\n");
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
