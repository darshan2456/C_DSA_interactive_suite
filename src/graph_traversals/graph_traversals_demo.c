#include "graph_traversals.h"
#include "safe_input.h"
#include <stdio.h>

void graph_traversals_demo(void)
{
    while (1)
    {
        int graph_traversal_choice;
        int graph_traversal_status = safe_input_int(
    &graph_traversal_choice,
    "\nenter 1 for bfs, 2 for dfs, 3 for dijkstra, 4 for astar, "
    "5 for greedy-bfs, 6 for bellman ford : ",
    1, 6);

        if (graph_traversal_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting graph traversal demo...\n");
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
        }
    }
}