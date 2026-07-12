#include <stdio.h>
#include <string.h>

#include "advanced_graph_algorithms.h"
#include "advanced_heaps.h"
#include "advanced_sorting.h"
#include "array.h"
#include "backtracking.h"
#include "benchmark.h"
#include "cache.h"
#include "compression.h"
#include "config.h"
#include "dcll.h"
#include "display_header.h"
#include "dll.h"
#include "dynamic_programming.h"
#include "error_correction_algorithms.h"
#include "expression.h"
#include "graph_traversals.h"
#include "hash.h"
#include "job_scheduling.h"
#include "memory_tracker.h"
#include "priority_queue.h"
#include "process_synchronization.h"
#include "queue.h"
#include "safe_input.h"
#include "scll.h"
#include "searching_algorithms.h"
#include "sll.h"
#include "sorting_algorithms_n2.h"
#include "stack.h"
#include "step_debugger.h"
#include "string_algorithms.h"
#include "trees.h"

#ifndef _WIN32
#include "tui.h"
#endif

void data_structures_demo(void);

// only give input as integer value as prompted through the console by programmer. dont attempt to
// put any other type of value neglecting this warning can result in undefined behaviour

void run_legacy_menu()
{
    while (1)
    {
        int choice;
        int status = safe_input_int(
            &choice, // variable in which value is to be inserted
            "\nWelcome to DSA library built by Darshan Mukul Parekh"
            "\n(at any point enter '-1' to exit that particular demo)\n\n"
            "click 1 for data structures demo\n"
            "click 2 for expression evaluation (infix to postfix and postfix evaluation) demo\n"
            "click 3 for sorting algorithms (the n^2 family) demo\n"
            "click 4 for advanced sorting algorithms demo\n"
            "click 5 for searching algorithms demo\n"
            "click 6 for graph traversals (bfs / dfs / dijkstra / astar / greedy bfs / bellman "
            "ford) demo\n"
            "click 7 for hashing algorithms demo\n"
            "click 8 for trees demo\n"
            "click 9 for error correction algorithms demo\n"
            "click 10 for job scheduling (FCFS / SJF / priority / round robin) demo\n"
            "click 11 for backtracking algorithms demo\n"
            "click 12 for dynamic programming algorithms demo\n"
            "click 13 for string algorithms demo\n"
            "click 14 for process synchronization algorithms demo\n"
            "click 15 for setting animation speed (by default 2s)\n"
            "click 16 for algorithm benchmarking and profiling demo\n"
            "click 17 for advanced graph algorithms (scc / max flow / bipartite matching / "
            "eulerian path) demo\n"
            "click 18 for advanced heaps & priority queues suite demo\n"
            "click 19 for interactive algorithm step-debugger demo\n"
            "click 20 for cache replacement simulator demo\n"
            "click 21 for interactive string compression & encoding suite demo\n"
            "enter choice : ",
            1, 21 // limits
        );

        if (status == -111)
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
                data_structures_demo();
                break;
            case 2:
                expression_evaluation_demo();
                break;
            case 3:
                sorting_algorithms_n2_demo();
                break;
            case 4:
                advanced_sorting_demo();
                break;
            case 5:
                searching_algorithms_demo();
                break;
            case 6:
                graph_traversals_demo();
                break;
            case 7:
                hashing_algorithms_demo();
                break;
            case 8:
                trees_demo();
                break;
            case 9:
                error_correction_algorithms_demo();
                break;
            case 10:
                job_scheduling_demo();
                break;
            case 11:
                backtracking_demo();
                break;
            case 12:
                dynamic_programming_demo();
                break;
            case 13:
                string_algorithms_demo();
                break;
            case 14:
                process_synchronization_demo();
                break;
            case 15:
                display_header("Settings");
                settings_menu_demo();
                break;
            case 16:
                display_header("Algorithm Benchmarking & Profiling");
                benchmark_menu_demo();
                break;
            case 17:
                advanced_graph_algorithms_demo();
                break;
            case 18:
                advanced_heaps_demo();
                break;
            case 19:
                debugger_demo();
                break;
            case 20:
                cache_simulator_demo();
                break;
            case 21:
                compression_demo();
                break;
        }
    }
}

#ifndef _WIN32
void tui_menu()
{
    while (1)
    {
        int choice;
        int status = safe_input_int(&choice,
                                    "\nWelcome to DSA library built by Darshan Mukul Parekh"
                                    "\n(at any point enter '-1' to exit that particular demo)\n\n"
                                    "click 1 for legacy menu\n"
                                    "click 2 for tui menu\n"
                                    "enter choice : ",
                                    1, 2 // limits
        );

        if (status == -111)
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
                run_legacy_menu();
                break;
            case 2:
                tui_run();
                break;
        }
    }
}
#endif

int main(int argc, char* argv[])
{
    init_windows_console();

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--profile") == 0)
        {
            init_memory_tracker();
            break;
        }
    }

#ifdef _WIN32
    run_legacy_menu();
#else
    tui_menu();
#endif

    return 0;
}
