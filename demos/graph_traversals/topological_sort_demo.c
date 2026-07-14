#include "graph_traversals.h"
#include "queue.h"
#include "returnMallocVal.h"
#include "safe_input.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void topological_sort_demo(void)
{
    int edges;
    int graph_capacity;
    Graph* graph = NULL;

    while (1)
    {
        int graph_capacity_status = safe_input_int(&graph_capacity,
                                                   "\nenter the number of vertices in the graph, "
                                                   "(between 1 and 10), enter '-1' to exit : ",
                                                   1, 10);

        if (graph_capacity_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting topological sort demo.....\n");
            free_graph(graph);
            return;
        }

        if (graph_capacity_status == 0)
        {
            continue;
        }

        graph = create_graph(graph_capacity);

        if (!graph)
        {
            printf("\nmemory allocation failed\n");
            free_graph(graph);
            return;
        }

        break;
    }

    while (1)
    {
        int edges_capacity_status = safe_input_int(
            &edges, "\nenter number of directed edges (between 1 and 100), enter '-1' to exit :", 1,
            100);

        if (edges_capacity_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting topological sort demo\n");
            free_graph(graph);
            return;
        }

        if (edges_capacity_status == 0)
        {
            continue;
        }

        break;
    }

    printf("\nenter directed edges (src dest) (from 0 to vertices-1, enter '-1' to exit):\n");

    for (int i = 0; i < edges; i++)
    {
        int src_status;
        int dest_status;
        int src;
        int dest;

    retry:
        src_status = safe_input_int(&src, "src: ", 0, graph_capacity - 1);

        if (src_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting topological sort demo\n");
            free_graph(graph);
            return;
        }
        if (src_status == 0)
        {
            goto retry;
        }

        dest_status = safe_input_int(&dest, "dest: ", 0, graph_capacity - 1);

        if (dest_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting topological sort demo\n");
            free_graph(graph);
            return;
        }
        if (dest_status == 0)
        {
            goto retry;
        }

        add_edge_directed_unweighted(graph, src, dest);
    }

    topological_sort_kahn(graph);
    free_graph(graph);
}