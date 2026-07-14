#include "graph_traversals.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void prim_demo(void)
{
    int edges;
    int graph_capacity = 0;
    int starting_node;
    int input_method;
    weightedGraph* graph = NULL;

    while (1)
    {
        int method_status = safe_input_int(&input_method,
                                           "\nEnter 1 to build the graph manually, 2 to load it "
                                           "from a CSV file, enter '-1' to exit: ",
                                           1, 2);

        if (method_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Prim's demo.....\n");
            return;
        }

        if (method_status == 0)
        {
            continue;
        }
        break;
    }

    if (input_method == 2)
    {
        while (1)
        {
            char path[256];
            printf("\nEnter the path to the CSV file, enter '-1' to exit: ");
            fflush(stdout);

            if (!fgets(path, sizeof(path), stdin))
            {
                printf("\nInput ended unexpectedly\n");
                clearerr(stdin);
                return;
            }

            size_t len = strlen(path);
            while (len > 0 && (path[len - 1] == '\n' || path[len - 1] == '\r'))
            {
                path[--len] = '\0';
            }

            if (strcmp(path, "-1") == 0)
            {
                printf("\nExiting Prim's demo.....\n");
                return;
            }

            if (len == 0)
            {
                continue;
            }

            graph = load_weightedGraph_from_csv(path);
            if (!graph)
            {
                continue;
            }
            break;
        }
        graph_capacity = graph->V;
    }
    else if (input_method == 1)
    {
        while (1)
        {
            int graph_capacity_status =
                safe_input_int(&graph_capacity,
                               "\nEnter the number of vertices in the graph (1-100), "
                               "enter '-1' to exit: ",
                               1, 100);

            if (graph_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Prim's demo.....\n");
                return;
            }
            if (graph_capacity_status == 0)
            {
                continue;
            }

            graph = create_weightedGraph(graph_capacity);
            if (!graph)
            {
                printf("\nMemory allocation failed\n");
                return;
            }
            break;
        }

        while (1)
        {
            int edges_status = safe_input_int(
                &edges, "\nEnter number of edges (0-1000), enter '-1' to exit: ", 0, 1000);

            if (edges_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Prim's demo.....\n");
                free_weightedGraph(graph);
                return;
            }
            if (edges_status == 0)
            {
                continue;
            }
            break;
        }

        printf(
            "\nEnter source, destination, weight pairs (Source, Destination between 0 and %d):\n",
            graph_capacity - 1);

        for (int i = 0; i < edges; i++)
        {
            int u, v, w;
            int status;

        retry_u:
            status = safe_input_int(&u, "  Source: ", 0, graph_capacity - 1);
            if (status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Prim's demo.....\n");
                free_weightedGraph(graph);
                return;
            }
            if (status == 0)
            {
                goto retry_u;
            }

        retry_v:
            status = safe_input_int(&v, "  Destination: ", 0, graph_capacity - 1);
            if (status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Prim's demo.....\n");
                free_weightedGraph(graph);
                return;
            }
            if (status == 0)
            {
                goto retry_v;
            }

        retry_w:
            status = safe_input_int(&w, "  Weight: ", 0, INT_MAX - 1);
            if (status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Prim's demo.....\n");
                free_weightedGraph(graph);
                return;
            }
            if (status == 0)
            {
                goto retry_w;
            }

            add_edge_directed(graph, u, v, w);
            add_edge_directed(graph, v, u, w);
        }
    }

    while (1)
    {
        int start_status =
            safe_input_int(&starting_node, "\nEnter starting node: ", 0, graph_capacity - 1);
        if (start_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Prim's demo.....\n");
            free_weightedGraph(graph);
            return;
        }
        if (start_status == 0)
        {
            continue;
        }

        if (starting_node < 0 || starting_node >= graph->V)
        {
            printf("Invalid start node\n");
            free_weightedGraph(graph);
            return;
        }
        break;
    }

    prim_mst(graph, starting_node);
    free_weightedGraph(graph);
}
