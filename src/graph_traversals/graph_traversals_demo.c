#include "graph_traversals.h"
#include "safe_input.h"
#include <stdio.h>

void graph_traversals_demo(void)
{
    while (1)
    {
        int graph_traversal_choice;
        int graph_traversal_status = safe_input_int(&graph_traversal_choice,
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
                                                    "\nEnter choice (-1 to return): ",
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
                bfs_demo();
                break;
            case 2:
                dfs_demo();
                break;
            case 3:
                dijkstra_demo();
                break;
            case 4:
                astar_demo();
                break;
            case 5:
                greedy_best_first_search_demo();
                break;
            case 6:
                bellman_ford_demo();
                break;
            case 7:
                topological_sort_demo();
                break;
            case 8:
                visualize_graph_demo();
                break;
            case 9:
                kruskal_demo();
                break;
            case 10:
                prim_demo();
                break;
            case 11:
                floyd_warshall_demo();
                break;
        }
    }
}
