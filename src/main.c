#include "advanced_sorting_algorithms/advanced_sorting.h"
#include "backtracking/backtracking.h"
#include "benchmark.h"
#include "data_structures/data_structures.h"
#include "dynamic_programming/dynamic_programming.h"
#include "error_correction_algorithms/error_correction_algorithms.h"
#include "expression_evaluation/expression.h"
#include "graph_traversals/graph_traversals.h"
#include "hashing/hash.h"
#include "job_scheduling/job_scheduling.h"
#include "process_synchronization/process_synchronization.h"
#include "searching_algorithms/searching_algorithms.h"
#include "sorting_algorithms_n2/sorting_algorithms_n2.h"
#include "string_algorithms/string_algorithms.h"
#include "trees/trees.h"
#include "utils/config.h"
#include "utils/display_header.h"
#include "utils/safe_input.h"
#include "../visualization/include/graph_visualizer.h"
#include "../visualization/include/tree_visualizer.h"
#ifndef _WIN32
#include "tui/tui.h"
#endif
#include <stdio.h>
#include <unistd.h>

void data_structures_demo(void);

// Visualization menu function prototype
void show_visualization_menu(void);

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
            "click 17 for visualization demos (Graphs & Trees)\n"  // <-- ADDED
            "enter choice : ",
            1, 17 // <-- CHANGED from 16 to 17
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
            case 17:  // <-- ADDED
                show_visualization_menu();
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

// ============================================================
// VISUALIZATION MENU IMPLEMENTATION
// ============================================================

void show_visualization_menu(void)
{
    while (1)
    {
        clear_screen();
        display_header("VISUALIZATION DEMOS");
        printf("\n");
        printf("  " BRIGHT_CYAN "╔══════════════════════════════════════════════════════════════╗\n" ANSI_RESET);
        printf("  " BRIGHT_CYAN "║" ANSI_RESET "  %-58s  " BRIGHT_CYAN "║\n" ANSI_RESET, "Graph & Tree Algorithm Visualization");
        printf("  " BRIGHT_CYAN "║" ANSI_RESET "  %-58s  " BRIGHT_CYAN "║\n" ANSI_RESET, "");
        printf("  " BRIGHT_CYAN "║" ANSI_RESET "  %-58s  " BRIGHT_CYAN "║\n" ANSI_RESET, "1. Graph BFS Visualization");
        printf("  " BRIGHT_CYAN "║" ANSI_RESET "  %-58s  " BRIGHT_CYAN "║\n" ANSI_RESET, "2. Graph DFS Visualization");
        printf("  " BRIGHT_CYAN "║" ANSI_RESET "  %-58s  " BRIGHT_CYAN "║\n" ANSI_RESET, "3. Dijkstra's Algorithm Visualization");
        printf("  " BRIGHT_CYAN "║" ANSI_RESET "  %-58s  " BRIGHT_CYAN "║\n" ANSI_RESET, "4. Tree Inorder Traversal");
        printf("  " BRIGHT_CYAN "║" ANSI_RESET "  %-58s  " BRIGHT_CYAN "║\n" ANSI_RESET, "5. Tree Preorder Traversal");
        printf("  " BRIGHT_CYAN "║" ANSI_RESET "  %-58s  " BRIGHT_CYAN "║\n" ANSI_RESET, "6. Tree Postorder Traversal");
        printf("  " BRIGHT_CYAN "║" ANSI_RESET "  %-58s  " BRIGHT_CYAN "║\n" ANSI_RESET, "7. Tree Level Order Traversal");
        printf("  " BRIGHT_CYAN "║" ANSI_RESET "  %-58s  " BRIGHT_CYAN "║\n" ANSI_RESET, "8. AVL Tree Operations");
        printf("  " BRIGHT_CYAN "║" ANSI_RESET "  %-58s  " BRIGHT_CYAN "║\n" ANSI_RESET, "0. Back to Main Menu");
        printf("  " BRIGHT_CYAN "╚══════════════════════════════════════════════════════════════╝\n" ANSI_RESET);
        printf("\n  Enter your choice: ");

        int choice;
        int status = safe_input_int(&choice, "", 0, 8);

        if (status == -111)
        {
            break;
        }

        if (status == 0)
        {
            continue;
        }

        // Initialize visualization
        VisualizerConfig config = {
            .animation_delay_ms = 300,
            .use_colors = true,
            .show_step_numbers = true,
            .highlight_path = true
        };
        visualizer_init(&config);

        switch (choice)
        {
            case 1:
            {
                // Graph BFS Visualization
                clear_screen();
                display_header("Graph BFS Visualization");
                printf("\n" BRIGHT_YELLOW "Creating a sample graph with 6 vertices...\n" ANSI_RESET);
                
                GraphVisualizer *gv = graph_visualizer_create(6);
                int matrix[6][6] = {
                    {0,1,1,0,0,0},
                    {1,0,0,1,1,0},
                    {1,0,0,0,0,1},
                    {0,1,0,0,0,0},
                    {0,1,0,0,0,1},
                    {0,0,1,0,1,0}
                };
                int *matrix_ptr[6];
                for (int i = 0; i < 6; i++) matrix_ptr[i] = matrix[i];
                graph_visualizer_set_adj_matrix(gv, (int**)matrix_ptr, 6);
                graph_visualizer_set_speed(gv, 500);
                
                printf("\n" BRIGHT_GREEN "Starting BFS from vertex 0...\n" ANSI_RESET);
                sleep(1);
                graph_visualizer_animate_bfs(gv, 0);
                graph_visualizer_free(gv);
                
                printf("\n" BRIGHT_YELLOW "Press Enter to continue..." ANSI_RESET);
                getchar();
                break;
            }
            case 2:
            {
                // Graph DFS Visualization
                clear_screen();
                display_header("Graph DFS Visualization");
                printf("\n" BRIGHT_YELLOW "Creating a sample graph with 6 vertices...\n" ANSI_RESET);
                
                GraphVisualizer *gv = graph_visualizer_create(6);
                int matrix[6][6] = {
                    {0,1,1,0,0,0},
                    {1,0,0,1,1,0},
                    {1,0,0,0,0,1},
                    {0,1,0,0,0,0},
                    {0,1,0,0,0,1},
                    {0,0,1,0,1,0}
                };
                int *matrix_ptr[6];
                for (int i = 0; i < 6; i++) matrix_ptr[i] = matrix[i];
                graph_visualizer_set_adj_matrix(gv, (int**)matrix_ptr, 6);
                graph_visualizer_set_speed(gv, 500);
                
                printf("\n" BRIGHT_GREEN "Starting DFS from vertex 0...\n" ANSI_RESET);
                sleep(1);
                graph_visualizer_animate_dfs(gv, 0);
                graph_visualizer_free(gv);
                
                printf("\n" BRIGHT_YELLOW "Press Enter to continue..." ANSI_RESET);
                getchar();
                break;
            }
            case 3:
            {
                // Dijkstra Visualization
                clear_screen();
                display_header("Dijkstra's Algorithm Visualization");
                printf("\n" BRIGHT_YELLOW "Creating a weighted graph with 6 vertices...\n" ANSI_RESET);
                
                GraphVisualizer *gv = graph_visualizer_create(6);
                int matrix[6][6] = {
                    {0,2,4,0,0,0},
                    {2,0,0,1,3,0},
                    {4,0,0,0,0,2},
                    {0,1,0,0,0,0},
                    {0,3,0,0,0,1},
                    {0,0,2,0,1,0}
                };
                int *matrix_ptr[6];
                for (int i = 0; i < 6; i++) matrix_ptr[i] = matrix[i];
                graph_visualizer_set_adj_matrix(gv, (int**)matrix_ptr, 6);
                graph_visualizer_set_speed(gv, 500);
                
                printf("\n" BRIGHT_GREEN "Finding shortest paths from vertex 0...\n" ANSI_RESET);
                sleep(1);
                graph_visualizer_animate_dijkstra(gv, 0);
                graph_visualizer_free(gv);
                
                printf("\n" BRIGHT_YELLOW "Press Enter to continue..." ANSI_RESET);
                getchar();
                break;
            }
            case 4:
            {
                // Tree Inorder Traversal
                clear_screen();
                display_header("Tree Inorder Traversal");
                printf("\n" BRIGHT_YELLOW "Creating a BST with 7 nodes...\n" ANSI_RESET);
                
                TreeVisualizer *tv = tree_visualizer_create();
                tv->animation_delay_ms = 400;
                int values[] = {50, 30, 70, 20, 40, 60, 80};
                for (int i = 0; i < 7; i++) tree_visualizer_insert(tv, values[i]);
                
                printf("\n" BRIGHT_GREEN "Starting Inorder Traversal...\n" ANSI_RESET);
                sleep(1);
                tree_visualizer_animate_inorder(tv);
                tree_visualizer_free(tv);
                
                printf("\n" BRIGHT_YELLOW "Press Enter to continue..." ANSI_RESET);
                getchar();
                break;
            }
            case 5:
            {
                // Tree Preorder Traversal
                clear_screen();
                display_header("Tree Preorder Traversal");
                printf("\n" BRIGHT_YELLOW "Creating a BST with 7 nodes...\n" ANSI_RESET);
                
                TreeVisualizer *tv = tree_visualizer_create();
                tv->animation_delay_ms = 400;
                int values[] = {50, 30, 70, 20, 40, 60, 80};
                for (int i = 0; i < 7; i++) tree_visualizer_insert(tv, values[i]);
                
                printf("\n" BRIGHT_GREEN "Starting Preorder Traversal...\n" ANSI_RESET);
                sleep(1);
                tree_visualizer_animate_preorder(tv);
                tree_visualizer_free(tv);
                
                printf("\n" BRIGHT_YELLOW "Press Enter to continue..." ANSI_RESET);
                getchar();
                break;
            }
            case 6:
            {
                // Tree Postorder Traversal
                clear_screen();
                display_header("Tree Postorder Traversal");
                printf("\n" BRIGHT_YELLOW "Creating a BST with 7 nodes...\n" ANSI_RESET);
                
                TreeVisualizer *tv = tree_visualizer_create();
                tv->animation_delay_ms = 400;
                int values[] = {50, 30, 70, 20, 40, 60, 80};
                for (int i = 0; i < 7; i++) tree_visualizer_insert(tv, values[i]);
                
                printf("\n" BRIGHT_GREEN "Starting Postorder Traversal...\n" ANSI_RESET);
                sleep(1);
                tree_visualizer_animate_postorder(tv);
                tree_visualizer_free(tv);
                
                printf("\n" BRIGHT_YELLOW "Press Enter to continue..." ANSI_RESET);
                getchar();
                break;
            }
            case 7:
            {
                // Tree Level Order Traversal
                clear_screen();
                display_header("Tree Level Order Traversal");
                printf("\n" BRIGHT_YELLOW "Creating a BST with 7 nodes...\n" ANSI_RESET);
                
                TreeVisualizer *tv = tree_visualizer_create();
                tv->animation_delay_ms = 400;
                int values[] = {50, 30, 70, 20, 40, 60, 80};
                for (int i = 0; i < 7; i++) tree_visualizer_insert(tv, values[i]);
                
                printf("\n" BRIGHT_GREEN "Starting Level Order Traversal...\n" ANSI_RESET);
                sleep(1);
                tree_visualizer_animate_level_order(tv);
                tree_visualizer_free(tv);
                
                printf("\n" BRIGHT_YELLOW "Press Enter to continue..." ANSI_RESET);
                getchar();
                break;
            }
            case 8:
            {
                // AVL Tree Operations
                clear_screen();
                display_header("AVL Tree Visualization");
                printf("\n" BRIGHT_YELLOW "Creating an AVL tree with balanced insertions...\n" ANSI_RESET);
                
                TreeVisualizer *tv = tree_visualizer_create();
                tv->animation_delay_ms = 400;
                
                // Insert values that will cause rotations
                int values[] = {10, 20, 30, 40, 50, 25};
                printf("\n" BRIGHT_GREEN "Inserting values: ");
                for (int i = 0; i < 6; i++) {
                    printf("%d ", values[i]);
                    fflush(stdout);
                    tree_visualizer_insert(tv, values[i]);
                    visualizer_clear();
                    display_header("AVL Tree - After Insertions");
                    tree_visualizer_draw(tv);
                    visualizer_delay(500);
                }
                printf("\n" ANSI_RESET);
                
                printf("\n" BRIGHT_GREEN "AVL tree built successfully!\n" ANSI_RESET);
                tree_visualizer_draw(tv);
                
                tree_visualizer_free(tv);
                printf("\n" BRIGHT_YELLOW "Press Enter to continue..." ANSI_RESET);
                getchar();
                break;
            }
            case 0:
                visualizer_cleanup();
                return;
            default:
                printf("\n" BRIGHT_RED "Invalid choice!\n" ANSI_RESET);
                sleep(1);
        }
        visualizer_cleanup();
    }
}

int main()
{
#ifdef _WIN32
    run_legacy_menu();
#else
    tui_menu();
#endif

    return 0;
}