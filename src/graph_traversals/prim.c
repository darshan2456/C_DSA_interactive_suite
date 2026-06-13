#include "graph_io.h"
#include "graph_traversals.h"
#include "history_logger.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * prim_mst - Implements Prim's algorithm to find the Minimum Spanning Tree (MST).
 * @graph: The weighted undirected graph.
 * @start_node: The starting node for the algorithm.
 *
 * Returns the total weight of the MST, or -1 on error.
 */
int prim_mst(weightedGraph* graph, int start_node)
{
    if (graph == NULL || start_node < 0 || start_node >= graph->V)
    {
        return -1;
    }

    int V = graph->V;
    int* key = (int*)malloc(V * sizeof(int));
    int* parent = (int*)malloc(V * sizeof(int));
    bool* in_mst = (bool*)malloc(V * sizeof(bool));

    if (!key || !parent || !in_mst)
    {
        free(key);
        free(parent);
        free(in_mst);
        return -1;
    }

    for (int i = 0; i < V; i++)
    {
        key[i] = INT_MAX;
        parent[i] = -1;
        in_mst[i] = false;
    }

    PQ_graph pq;
    init_pq_graph(&pq, V);
    if (!pq.heap)
    {
        free(key);
        free(parent);
        free(in_mst);
        return -1;
    }

    key[start_node] = 0;
    if(!insert_pq_graph(&pq, start_node, 0))
    {
        printf("Malloc failed\n");
        free(key);
        free(parent);
        free(in_mst);
        return -1;
    }

    int mst_weight = 0;
    int nodes_in_mst = 0;

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();

    printf("\nEdges in the Minimum Spanning Tree (Prim's Algorithm):\n");
    printf("Source -- Destination == Weight\n");
    printf("------------------------------\n");

    while (pq.size > 0)
    {
        PQ_graph_node top;
        if (!extractTop_pq_graph(&pq, &top))
        {
            break;
        }

        int u = top.vertex;

        if (in_mst[u])
        {
            continue;
        }

        in_mst[u] = true;
        mst_weight += top.distance;
        nodes_in_mst++;

        if (parent[u] != -1)
        {
            printf("  %d    --      %d      ==   %d\n", parent[u], u, top.distance);
        }

        Edge* curr = graph->array[u];
        while (curr)
        {
            int v = curr->destination;
            int weight = curr->weight;

            if (!in_mst[v] && weight < key[v])
            {
                key[v] = weight;
                parent[v] = u;
                if(!insert_pq_graph(&pq, v, key[v]))
                {
                    printf("Malloc failed\n");
                    free(key);
                    free(parent);
                    free(in_mst);
                    return -1;
                }
            }
            curr = curr->next;
        }
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    if (nodes_in_mst < V)
    {
        printf("\nWarning: The graph is disconnected. The result is a Minimum Spanning Forest.\n");
    }

    printf("\nTotal Weight of MST: %d\n", mst_weight);
    printf("Total CPU time taken: %f seconds\n", total_t);

    add_to_history("Prim's MST", V, total_t);

    free_pq_graph(&pq);
    free(key);
    free(parent);
    free(in_mst);

    return mst_weight;
}

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
            status = safe_input_int(&w, "  Weight: ", 0, INT_MAX);
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
