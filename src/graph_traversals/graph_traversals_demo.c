#include "advanced_graph_algorithms.h"
#include "display_header.h"
#include "graph_traversals.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>

void graph_traversals_demo(void)
{
    while (1)
    {
        display_header("Graph Traversals");

        int graph_traversal_choice;
        int graph_traversal_status =
            safe_input_int(&graph_traversal_choice,
                           "\nGraph Algorithms Demo\n"
                           "---------------------\n"
                           "\n"
                           "1. BFS\n"
                           "2. DFS\n"
                           "3. Dijkstra\n"
                           "4. A*\n"
                           "5. Greedy BFS\n"
                           "6. Bellman-Ford\n"
                           "7. Topological Sort\n"
                           "8. Visualize Graph\n"
                           "9. Kruskal MST\n"
                           "10. Prim MST\n"
                           "11. Floyd-Warshall\n"
                           "\nenter choice (\'-1\' to exit, or \'help\') : ",
                           1, 11);

        if (graph_traversal_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting graph algorithms demo...\n");
            return;
        }

        if (graph_traversal_status == 0)
        {
            continue;
        }

        switch (graph_traversal_choice)
        {
            case 1:
                display_header("BFS");
                bfs_demo();
                break;
            case 2:
                display_header("DFS");
                dfs_demo();
                break;
            case 3:
                display_header("Dijkstra");
                dijkstra_demo();
                break;
            case 4:
                display_header("A*");
                astar_demo();
                break;
            case 5:
                display_header("Greedy Best-First Search");
                greedy_best_first_search_demo();
                break;
            case 6:
                display_header("Bellman-Ford");
                bellman_ford_demo();
                break;
            case 7:
                display_header("Topological Sort");
                topological_sort_demo();
                break;
            case 8:
                display_header("Visualize Graph");
                visualize_graph_demo();
                break;
            case 9:
                display_header("Kruskal MST");
                kruskal_demo();
                break;
            case 10:
                display_header("Prim MST");
                prim_demo();
                break;
            case 11:
                display_header("Floyd-Warshall");
                floyd_warshall_demo();
                break;
        }
    }
}
