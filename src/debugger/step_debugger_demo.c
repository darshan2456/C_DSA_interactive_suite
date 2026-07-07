#include "display_header.h"
#include "graph_traversals.h"
#include "safe_input.h"
#include "sorting_algorithms_n2.h"
#include "step_debugger.h"
#include <stdio.h>
#include <stdlib.h>

// Forward declarations for bfs and dfs as they are not in the main header
extern void bfs(Graph* graph, int start);
extern void dfs(Graph* graph, int start);

void debugger_demo(void)
{
    while (1)
    {
        display_header("Interactive Algorithm Step-Debugger");

        int choice;
        int status = safe_input_int(&choice,
                                    "\n--- Step-Debugger Algorithms Demo ---\n"
                                    "1. Debug Bubble Sort\n"
                                    "2. Debug Selection Sort\n"
                                    "3. Debug Insertion Sort\n"
                                    "4. Debug Breadth-First Search (BFS)\n"
                                    "5. Debug Depth-First Search (DFS)\n"
                                    "6. Debug Dijkstra's Shortest Path\n"
                                    "\nEnter choice (-1 to exit): ",
                                    1, 6);

        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Step-Debugger menu...\n");
            return;
        }
        if (status == 0)
        {
            continue;
        }

        // Enable step mode and start paused for the interactive session
        set_step_mode(1);
        set_paused(1);

        switch (choice)
        {
            case 1:
            {
                display_header("Debugging Bubble Sort");
                int arr[] = {5, 2, 8, 1, 9};
                int length = sizeof(arr) / sizeof(arr[0]);
                printf("Initial array: [5, 2, 8, 1, 9]\n");
                bubble_sort_optimized(arr, length);
                printf("\nSorted array: ");
                for (int i = 0; i < length; i++)
                    printf("%d ", arr[i]);
                printf("\n");
                break;
            }
            case 2:
            {
                display_header("Debugging Selection Sort");
                int arr[] = {5, 2, 8, 1, 9};
                int length = sizeof(arr) / sizeof(arr[0]);
                printf("Initial array: [5, 2, 8, 1, 9]\n");
                selection_sort(arr, length);
                printf("\nSorted array: ");
                for (int i = 0; i < length; i++)
                    printf("%d ", arr[i]);
                printf("\n");
                break;
            }
            case 3:
            {
                display_header("Debugging Insertion Sort");
                int arr[] = {5, 2, 8, 1, 9};
                int length = sizeof(arr) / sizeof(arr[0]);
                printf("Initial array: [5, 2, 8, 1, 9]\n");
                insertion_sort(arr, length);
                printf("\nSorted array: ");
                for (int i = 0; i < length; i++)
                    printf("%d ", arr[i]);
                printf("\n");
                break;
            }
            case 4:
            {
                display_header("Debugging BFS");
                Graph* g = create_graph(5);
                add_edge_undirected(g, 0, 1);
                add_edge_undirected(g, 0, 2);
                add_edge_undirected(g, 1, 3);
                add_edge_undirected(g, 2, 4);
                printf("Running BFS traversal starting at vertex 0 on a 5-vertex graph...\n");
                bfs(g, 0);
                free_graph(g);
                break;
            }
            case 5:
            {
                display_header("Debugging DFS");
                Graph* g = create_graph(5);
                add_edge_undirected(g, 0, 1);
                add_edge_undirected(g, 0, 2);
                add_edge_undirected(g, 1, 3);
                add_edge_undirected(g, 2, 4);
                printf("Running DFS traversal starting at vertex 0 on a 5-vertex graph...\n");
                dfs(g, 0);
                free_graph(g);
                break;
            }
            case 6:
            {
                display_header("Debugging Dijkstra");
                weightedGraph* wg = create_weightedGraph(5);
                add_edge_directed(wg, 0, 1, 4);
                add_edge_directed(wg, 0, 2, 2);
                add_edge_directed(wg, 1, 2, 3);
                add_edge_directed(wg, 1, 3, 2);
                add_edge_directed(wg, 1, 4, 3);
                add_edge_directed(wg, 2, 1, 1);
                add_edge_directed(wg, 2, 3, 4);
                add_edge_directed(wg, 2, 4, 5);
                add_edge_directed(wg, 4, 3, 1);
                printf("Running Dijkstra starting at vertex 0 on a 5-vertex weighted graph...\n");
                dijkstra(wg, 0);
                free_weightedGraph(wg);
                break;
            }
        }

        // Clean up debugger state
        set_step_mode(0);
        set_paused(0);

        printf("\nPress Enter to continue...");
        // Consume any leftover input
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        getchar();
    }
}
