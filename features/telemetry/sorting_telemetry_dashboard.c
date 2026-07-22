#include "advanced_sorting.h"
#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include "sorting_telemetry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void generate_dataset(int arr[], int n, int type)
{
    if (type == 1) // Random
    {
        for (int i = 0; i < n; i++)
        {
            arr[i] = rand() % (n * 10 + 1);
        }
    }
    else if (type == 2) // Sorted
    {
        for (int i = 0; i < n; i++)
        {
            arr[i] = i * 2;
        }
    }
    else if (type == 3) // Reverse-Sorted
    {
        for (int i = 0; i < n; i++)
        {
            arr[i] = (n - i) * 2;
        }
    }
}

void sorting_telemetry_dashboard_demo(void)
{
    while (1)
    {
        printf("\n========================================================================\n");
        printf("       UNIFIED SORTING TELEMETRY & PROFILING COMPARATIVE DASHBOARD       \n");
        printf("========================================================================\n");
        printf("1. Run Benchmark on Random Dataset\n");
        printf("2. Run Benchmark on Sorted Dataset\n");
        printf("3. Run Benchmark on Reverse-Sorted Dataset\n");
        printf("-1. Return to Main Menu\n");

        int choice;
        int status = safe_input_int(&choice, "\nEnter your choice: ", 1, 3);
        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Telemetry Dashboard...\n");
            return;
        }
        if (status == 0)
        {
            continue;
        }

        int n;
        status = safe_input_int(&n, "Enter dataset size N (10 to 5000): ", 10, 5000);
        if (status == INPUT_EXIT_SIGNAL)
        {
            return;
        }

        int* master_arr = malloc(sizeof(int) * n);
        int* test_arr = malloc(sizeof(int) * n);
        if (!master_arr || !test_arr)
        {
            printf("Memory allocation error!\n");
            if (master_arr)
                free(master_arr);
            if (test_arr)
                free(test_arr);
            return;
        }

        srand((unsigned int)time(NULL));
        generate_dataset(master_arr, n, choice);

        const char* dataset_name = (choice == 1)   ? "Random"
                                   : (choice == 2) ? "Sorted"
                                                   : "Reverse-Sorted";
        printf("\nRunning Telemetry Profiling on %s Dataset (N = %d)...\n", dataset_name, n);

        SortingTelemetry results[9];

        // Advanced Algorithms
        memcpy(test_arr, master_arr, sizeof(int) * n);
        quicksort_with_telemetry(test_arr, 0, n - 1, &results[0]);

        memcpy(test_arr, master_arr, sizeof(int) * n);
        merge_sort_with_telemetry(test_arr, n, &results[1]);

        memcpy(test_arr, master_arr, sizeof(int) * n);
        heap_sort_with_telemetry(test_arr, n, &results[2]);

        memcpy(test_arr, master_arr, sizeof(int) * n);
        radix_sort_with_telemetry(test_arr, n, &results[3]);

        memcpy(test_arr, master_arr, sizeof(int) * n);
        bucket_sort_with_telemetry(test_arr, n, &results[4]);

        // N^2 Algorithms
        memcpy(test_arr, master_arr, sizeof(int) * n);
        bubble_sort_optimized_with_telemetry(test_arr, n, &results[5]);

        memcpy(test_arr, master_arr, sizeof(int) * n);
        insertion_sort_with_telemetry(test_arr, n, &results[6]);

        memcpy(test_arr, master_arr, sizeof(int) * n);
        selection_sort_with_telemetry(test_arr, n, &results[7]);

        memcpy(test_arr, master_arr, sizeof(int) * n);
        shell_sort_with_telemetry(test_arr, n, &results[8]);

        free(master_arr);
        free(test_arr);

        // Display Comparative Matrix
        printf("\n+--------------------+---------------+--------------+------------+------------+---------------+------------+\n");
        printf("| Algorithm          | Time (us)     | Comparisons  | Swaps      | Copies     | Max Rec Depth | Pass Count |\n");
        printf("+--------------------+---------------+--------------+------------+------------+---------------+------------+\n");

        for (int i = 0; i < 9; i++)
        {
            printf("| %-18s | %13.2f | %12lld | %10lld | %10lld | %13d | %10d |\n",
                   results[i].algorithm_name,
                   results[i].elapsed_microseconds,
                   results[i].comparisons,
                   results[i].swaps,
                   results[i].copies,
                   results[i].max_recursion_depth,
                   results[i].pass_count);
        }
        printf("+--------------------+---------------+--------------+------------+------------+---------------+------------+\n");
    }
}
