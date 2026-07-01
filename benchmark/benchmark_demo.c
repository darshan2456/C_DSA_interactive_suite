#include "benchmark.h"
#include "display_header.h"
#include "safe_input.h"
#include <stdio.h>

void benchmark_menu_demo(void)
{
    while (1)
    {
        display_header("Algorithm Benchmarking & Profiling");

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
                                    "click 11 for Network Flow Algorithms benchmark\n"
                                    "click 12 for Advanced Heaps benchmark\n"
                                    "enter choice : ",
                                    1, 12);

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
        int n_status = safe_input_int(
            &n, "Enter input size N (between 10 and 100000), or -1 to exit: ", 10, 100000);
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
                run_mst_benchmark(n);
                break;
            case 5:
                run_scheduling_benchmark(n);
                break;
            case 6:
                run_strings_benchmark(n);
                break;
            case 7:
                run_dp_benchmark(n);
                break;
            case 8:
                run_hashing_benchmark(n);
                break;
            case 9:
                run_trees_benchmark(n);
                break;
            case 10:
                run_backtracking_benchmark(n);
                break;
            case 11:
                run_flow_benchmark(n);
                break;
            case 12:
                run_heaps_benchmark(n);
                break;
        }
    }
}
