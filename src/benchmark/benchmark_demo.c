#include "benchmark.h"
#include "safe_input.h"
#include <stdio.h>

void benchmark_menu_demo(void)
{
    while (1)
    {
        int choice;
        int status = safe_input_int(&choice,
                                    "\n--- Algorithm Benchmarking & Profiling Menu ---\n"
                                    "click 1 for Sorting Algorithms benchmark\n"
                                    "click 2 for Searching Algorithms benchmark\n"
                                    "click 3 for Graph Shortest Path Algorithms benchmark\n"
                                    "click 4 for MST Algorithms benchmark\n"
                                    "click 5 for Job Scheduling Algorithms benchmark\n"
                                    "click 6 for String Matching Algorithms benchmark\n"
                                    "click 7 for Dynamic Programming vs Naive Recursion benchmark\n"
                                    "click 8 for Hash Map Collision Resolution benchmark\n"
                                    "click 9 for Trees Lookup Performance benchmark\n"
                                    "click 10 for Backtracking Algorithms benchmark\n"
                                    "enter choice : ",
                                    1, 10);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Benchmark menu...\n");
            return;
        }
        if (status == 0)
        {
            continue;
        }

        int n;
        int n_status = safe_input_int(&n, "Enter input size N (between 10 and 100000), or -1 to exit: ", 10, 100000);
        if (n_status == INPUT_EXIT_SIGNAL)
        {
            continue;
        }
        if (n_status == 0)
        {
            continue;
        }

        switch (choice)
        {
            case 1:
                run_sorting_benchmark(n);
                break;
            case 2:
                run_searching_benchmark(n);
                break;
            case 3:
                run_graphs_benchmark(n);
                break;
            case 4:
                printf("\nMST benchmark selected for N = %d\n", n);
                printf("Benchmark module not implemented yet. Implementing in PR 6.\n");
                break;
            case 5:
                printf("\nJob Scheduling benchmark selected for N = %d\n", n);
                printf("Benchmark module not implemented yet. Implementing in PR 7.\n");
                break;
            case 6:
                printf("\nString Matching benchmark selected for N = %d\n", n);
                printf("Benchmark module not implemented yet. Implementing in PR 8.\n");
                break;
            case 7:
                printf("\nDynamic Programming vs Recursion benchmark selected for N = %d\n", n);
                printf("Benchmark module not implemented yet. Implementing in PR 9.\n");
                break;
            case 8:
                printf("\nHash Map Collision benchmark selected for N = %d\n", n);
                printf("Benchmark module not implemented yet. Implementing in PR 10.\n");
                break;
            case 9:
                printf("\nTrees Performance benchmark selected for N = %d\n", n);
                printf("Benchmark module not implemented yet. Implementing in PR 11.\n");
                break;
            case 10:
                printf("\nBacktracking benchmark selected for N = %d\n", n);
                printf("Benchmark module not implemented yet. Implementing in PR 12.\n");
                break;
        }
    }
}
