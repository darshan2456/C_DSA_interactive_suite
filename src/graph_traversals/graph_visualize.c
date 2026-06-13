#include "graph_io.h"
#include "graph_traversals.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <string.h>

// Builds a weighted graph (manual entry or CSV import) the same way the weighted
// search demos do, then prints it as an adjacency list. Kept separate from the
// unweighted path so each flow stays easy to follow.
static void visualize_weighted_graph(void)
{
    int edges;
    int graph_capacity;
    int input_method;
    weightedGraph* graph = NULL;

    while (1)
    {
        int method_status = safe_input_int(&input_method,
                                           "\nenter 1 to build the graph manually, 2 to load it "
                                           "from a CSV file, enter '-1' to exit : ",
                                           1, 2);

        if (method_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting graph visualization.....\n");
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
            printf("\nenter the path to the CSV file, enter '-1' to exit : ");
            fflush(stdout);

            if (!fgets(path, sizeof(path), stdin))
            {
                printf("\ninput ended unexpectedly\n");
                clearerr(stdin);
                return;
            }

            size_t len = strlen(path);
            while (len > 0 && (path[len - 1] == '\n' || path[len - 1] == '\r'))
                path[--len] = '\0';

            if (strcmp(path, "-1") == 0)
            {
                printf("\nExiting graph visualization.....\n");
                return;
            }

            if (len == 0)
            {
                continue;
            }

            graph = load_weightedGraph_from_csv(path);

            if (!graph)
            {
                // error already reported by the loader, let the user retry
                continue;
            }

            break;
        }
    }
    else if (input_method == 1)
    {
        while (1)
        {
            int graph_capacity_status =
                safe_input_int(&graph_capacity,
                               "\nenter the number of vertices in the graph, "
                               "(between 1 and 10), enter '-1' to exit : ",
                               1, 10);

            if (graph_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting graph visualization.....\n");
                return;
            }

            if (graph_capacity_status == 0)
            {
                continue;
            }

            graph = create_weightedGraph(graph_capacity);

            if (!graph)
            {
                printf("\nmalloc allocation failed\n");
                return;
            }

            break;
        }

        while (1)
        {
            int edges_capacity_status = safe_input_int(
                &edges, "\nenter number of edges (between 1 and 100), enter '-1' to exit :", 1,
                100);

            if (edges_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting graph visualization\n");
                free_weightedGraph(graph);
                return;
            }

            if (edges_capacity_status == 0)
            {
                continue;
            }

            break;
        }

        printf("\nenter edges (src dest weight) (vertices from 0 to %d, enter '-1' to exit):\n",
               graph_capacity - 1);

        for (int i = 0; i < edges; i++)
        {
            int src_status;
            int dest_status;
            int wt_status;
            int src;
            int dest;
            int wt;

        retry:
            src_status = safe_input_int(&src, "src: ", 0, graph_capacity - 1);

            if (src_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting graph visualization\n");
                free_weightedGraph(graph);
                return;
            }
            if (src_status == 0)
            {
                goto retry;
            }

            dest_status = safe_input_int(&dest, "dest: ", 0, graph_capacity - 1);

            if (dest_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting graph visualization\n");
                free_weightedGraph(graph);
                return;
            }
            if (dest_status == 0)
            {
                goto retry;
            }

            wt_status = safe_input_int(&wt, "weight: ", 0, INT_MAX);

            if (wt_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting graph visualization\n");
                free_weightedGraph(graph);
                return;
            }
            if (wt_status == 0)
            {
                goto retry;
            }

            add_edge_directed(graph, src, dest, wt);
        }
    }

    print_weightedGraph(graph);
    free_weightedGraph(graph);
}

// Standalone "visualize graph" demo: first asks whether to visualize an
// unweighted or weighted graph, then builds it the same two ways the traversal
// demos do (manual entry or CSV import) and prints it as an adjacency list. It
// deliberately shows the structure itself, separate from any traversal output.
void visualize_graph_demo(void)
{
    int edges;
    int graph_capacity;
    int input_method;
    Graph* graph = NULL;

    int graph_type;
    while (1)
    {
        int type_status = safe_input_int(&graph_type,
                                         "\nenter 1 to visualize an unweighted graph, 2 for a "
                                         "weighted graph, enter '-1' to exit : ",
                                         1, 2);

        if (type_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting graph visualization.....\n");
            return;
        }

        if (type_status == 0)
        {
            continue;
        }

        break;
    }

    if (graph_type == 2)
    {
        visualize_weighted_graph();
        return;
    }

    while (1)
    {
        int method_status = safe_input_int(&input_method,
                                           "\nenter 1 to build the graph manually, 2 to load it "
                                           "from a CSV file, enter '-1' to exit : ",
                                           1, 2);

        if (method_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting graph visualization.....\n");
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
            printf("\nenter the path to the CSV file, enter '-1' to exit : ");
            fflush(stdout);

            if (!fgets(path, sizeof(path), stdin))
            {
                printf("\ninput ended unexpectedly\n");
                clearerr(stdin);
                return;
            }

            size_t len = strlen(path);
            while (len > 0 && (path[len - 1] == '\n' || path[len - 1] == '\r'))
                path[--len] = '\0';

            if (strcmp(path, "-1") == 0)
            {
                printf("\nExiting graph visualization.....\n");
                return;
            }

            if (len == 0)
            {
                continue;
            }

            graph = load_graph_from_csv(path);

            if (!graph)
            {
                // error already reported by the loader, let the user retry
                continue;
            }

            break;
        }
    }
    else if (input_method == 1)
    {
        while (1)
        {
            int graph_capacity_status =
                safe_input_int(&graph_capacity,
                               "\nenter the number of vertices in the graph, "
                               "(between 1 and 10), enter '-1' to exit : ",
                               1, 10);

            if (graph_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting graph visualization.....\n");
                return;
            }

            if (graph_capacity_status == 0)
            {
                continue;
            }

            graph = create_graph(graph_capacity);

            if (!graph)
            {
                printf("\nmalloc allocation failed\n");
                return;
            }

            break;
        }

        while (1)
        {
            int edges_capacity_status = safe_input_int(
                &edges, "\nenter number of edges (between 1 and 100), enter '-1' to exit :", 1,
                100);

            if (edges_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting graph visualization\n");
                free_graph(graph);
                return;
            }

            if (edges_capacity_status == 0)
            {
                continue;
            }

            break;
        }

        printf("\nenter edges (src dest) (from 0 to vertices-1, enter '-1' to exit):\n");

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
                printf("\nExiting graph visualization\n");
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
                printf("\nExiting graph visualization\n");
                free_graph(graph);
                return;
            }
            if (dest_status == 0)
            {
                goto retry;
            }

            add_edge_undirected(graph, src, dest);
        }
    }

    print_graph(graph);
    free_graph(graph);
}
