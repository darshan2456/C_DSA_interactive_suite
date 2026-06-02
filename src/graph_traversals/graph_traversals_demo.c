#include "graph_traversals.h"
#include "safe_input.h"
#include <stdio.h>

void graph_traversals_demo(void)
{
    while (1)
    {
        int graph_traversal_choice;
        int graph_traversal_status = safe_input_int(
            &graph_traversal_choice, "\nenter 1 for bfs, 2 for dfs and 3 for dijkstra : ", 1, 3);

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
        }
    }
}