#include "graph_traversals.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void kruskal_demo(void)
{
    int V, E;
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
            printf("\nExiting Kruskal's demo.....\n");
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
                printf("\nExiting Kruskal's demo.....\n");
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
    }
    else if (input_method == 1)
    {
        while (1)
        {
            int status = safe_input_int(
                &V, "\nEnter number of vertices (1-100), enter '-1' to exit: ", 1, 100);
            if (status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Kruskal's demo.....\n");
                return;
            }
            if (status == 0)
            {
                continue;
            }
            break;
        }

        while (1)
        {
            int status = safe_input_int(
                &E, "\nEnter number of edges (0-1000), enter '-1' to exit: ", 0, 1000);
            if (status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Kruskal's demo.....\n");
                return;
            }
            if (status == 0)
            {
                continue;
            }
            break;
        }

        graph = create_weightedGraph(V);
        if (!graph)
        {
            return;
        }

        printf("\nEnter source, destination, weight triplets (between 0 and %d):\n", V - 1);
        for (int i = 0; i < E; i++)
        {
            int u, v, w;
            int status;

        retry_u:
            status = safe_input_int(&u, "  Source: ", 0, V - 1);
            if (status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Kruskal's demo.....\n");
                free_weightedGraph(graph);
                return;
            }
            if (status == 0)
            {
                goto retry_u;
            }

        retry_v:
            status = safe_input_int(&v, "  Destination: ", 0, V - 1);
            if (status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Kruskal's demo.....\n");
                free_weightedGraph(graph);
                return;
            }
            if (status == 0)
            {
                goto retry_v;
            }

        retry_w:
            status = safe_input_int(&w, "  Weight: ", 0, 10000);
            if (status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Kruskal's demo.....\n");
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

    kruskal_mst(graph);
    free_weightedGraph(graph);
}
