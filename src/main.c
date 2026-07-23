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
#include "fuzzer.h"
#include "graph_traversals.h"
#include "hash.h"
#include "job_scheduling.h"
#include "memory_inspector.h"
#include "memory_tracker.h"
#include "priority_queue.h"
#include "process_synchronization.h"
#include "queue.h"
#include "safe_input.h"
#include "scll.h"
#include "searching_algorithms.h"
#include "serialization.h"
#include "sll.h"
#include "sorting_algorithms_n2.h"
#include "stack.h"
#include "step_debugger.h"
#include "string_algorithms.h"
#include "trees.h"

#include "tui.h"

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
            "\n(at any point enter '-1' to exit that particular demo, or 'help' for manual)\n\n"
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
            "click 15 for advanced graph algorithms (scc / max flow / bipartite matching / "
            "eulerian path) demo\n"
            "click 16 for advanced heaps & priority queues suite demo\n"
            "click 17 for cache replacement simulator demo\n"
            "click 18 for interactive string compression & encoding suite demo\n"
            "click 19 for algorithm benchmarking and profiling demo\n"
            "click 20 for interactive algorithm step-debugger demo\n"
            "click 21 for setting animation speed (by default 2s)\n"
            "click 22 for Stochastic Fuzz Testing Engine demo\n"
            "click 23 for Raw Memory Layout Inspector / Hexdump Visualizer demo\n"
            "\nenter choice (\'-1\' to exit, or \'help\') : ",
            1, 23 // limits
        );

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
                advanced_graph_algorithms_demo();
                break;
            case 16:
                advanced_heaps_demo();
                break;
            case 17:
                cache_simulator_demo();
                break;
            case 18:
                compression_demo();
                break;
            case 19:
                display_header("Algorithm Benchmarking & Profiling");
                benchmark_menu_demo();
                break;
            case 20:
                debugger_demo();
                break;
            case 21:
                display_header("Settings");
                settings_menu_demo();
                break;
            case 22:
                display_header("Stochastic Fuzz Testing Engine");
                fuzzer_demo();
                break;
            case 23:
                display_header("Raw Memory Layout Inspector");
                memory_inspector_demo();
                break;
        }
    }
}

void tui_menu()
{
    while (1)
    {
        int choice;
        int status = safe_input_int(
            &choice,
            "\nWelcome to DSA library built by Darshan Mukul Parekh"
            "\n(at any point enter '-1' to exit that particular demo, or 'help' for manual)\n\n"
            "click 1 for legacy menu\n"
            "click 2 for tui menu\n"
            "\nenter choice (\'-1\' to exit, or \'help\') : ",
            1, 2 // limits
        );

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
                run_legacy_menu();
                break;
            case 2:
                tui_run();
                break;
        }
    }
}

int main(int argc, char* argv[])
{
    init_windows_console();

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--profile") == 0)
        {
            init_memory_tracker();
        }
        else if (strcmp(argv[i], "--export-trace") == 0)
        {
            set_telemetry_trace_enabled(1);
        }
        else if (strcmp(argv[i], "--export-trace-path") == 0 && i + 1 < argc)
        {
            set_telemetry_trace_enabled(1);
            set_telemetry_trace_filepath(argv[i + 1]);
            i++;
        }
        else if (strcmp(argv[i], "--load-bst") == 0 && i + 1 < argc)
        {
            const char* path = argv[i + 1];
            bstNode* root = deserialize_bst_from_file(path);
            if (root)
            {
                printf("BST loaded successfully from %s\n", path);
                printf("Inorder traversal: ");
                bst_inorder(root);
                printf("\nPreorder traversal: ");
                bst_preorder(root);
                printf("\nPostorder traversal: ");
                bst_postorder(root);
                printf("\n");
                destroy_bst(root);
            }
            else
            {
                printf("Failed to load BST from %s\n", path);
            }
            return 0;
        }
        else if (strcmp(argv[i], "--load-avl") == 0 && i + 1 < argc)
        {
            const char* path = argv[i + 1];
            avlNode* root = deserialize_avl_from_file(path);
            if (root)
            {
                printf("AVL tree loaded successfully from %s\n", path);
                printf("Inorder traversal: ");
                avl_inorder(root);
                printf("\nPreorder traversal: ");
                avl_preorder(root);
                printf("\nPostorder traversal: ");
                avl_postorder(root);
                printf("\n");
                destroy_avl(root);
            }
            else
            {
                printf("Failed to load AVL tree from %s\n", path);
            }
            return 0;
        }
        else if (strcmp(argv[i], "--load-graph") == 0 && i + 1 < argc)
        {
            const char* path = argv[i + 1];
            Graph* g = deserialize_graph_from_file(path);
            if (g)
            {
                printf("Graph loaded successfully from %s\n", path);
                print_graph(g);
                free_graph(g);
            }
            else
            {
                printf("Failed to load Graph from %s\n", path);
            }
            return 0;
        }
        else if (strcmp(argv[i], "--load-wgraph") == 0 && i + 1 < argc)
        {
            const char* path = argv[i + 1];
            weightedGraph* g = deserialize_weighted_graph_from_file(path);
            if (g)
            {
                printf("Weighted Graph loaded successfully from %s\n", path);
                print_weightedGraph(g);
                free_weightedGraph(g);
            }
            else
            {
                printf("Failed to load Weighted Graph from %s\n", path);
            }
            return 0;
        }
    }

    tui_menu();

    return 0;
}
