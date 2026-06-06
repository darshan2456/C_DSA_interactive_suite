#include "graph_traversals.h"
#include "safe_input.h"
#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Prim's Minimum Spanning Tree
 *
 * Uses the existing weightedGraph adjacency list and
 * project priority queue implementation.
 *
 * Time Complexity : O(E log V)
 * Space Complexity: O(V)
 *
 * Note: uses lazy insertion into the priority queue (same pattern as
 * dijkstra). Stale entries are discarded via the visited[] check.
 * Worst-case inserts = V*(V-1) directed relaxations for a dense
 * undirected graph, so HEAP_CAPACITY must be at least V*(V-1).
 */
void prim_mst(weightedGraph* graph, int start)
{
    if (graph == NULL)
        return;

    if (start < 0 || start >= graph->V)
    {
        fprintf(stderr, "Invalid starting vertex\n");
        return;
    }

    int vertices = graph->V;

    /* V*(V-1) is the maximum number of lazy inserts for a dense undirected
     * graph with V vertices. Catch mismatches at compile time. */
    static_assert(HEAP_CAPACITY >= 10 * 9,
                  "HEAP_CAPACITY too small for maximum supported graph size");

    int*  parent  = malloc(vertices * sizeof(int));
    int*  key     = malloc(vertices * sizeof(int));
    bool* visited = malloc(vertices * sizeof(bool));

    if (parent == NULL || key == NULL || visited == NULL)
    {
        fprintf(stderr, "\nMemory allocation failed\n");
        free(parent);
        free(key);
        free(visited);
        return;
    }

    for (int i = 0; i < vertices; i++)
    {
        parent[i]  = -1;
        key[i]     = INT_MAX;
        visited[i] = false;
    }

    key[start] = 0;

    PQ_graph pq;
    pq.size = 0;
    insert_pq_graph(&pq, start, 0);

    PQ_graph_node current;

    /*
     * Greedily expands the tree by repeatedly selecting the
     * minimum-weight edge that connects a visited vertex to
     * an unvisited vertex.
     */
    while (extractTop_pq_graph(&pq, &current))
    {
        int u = current.vertex;

        if (visited[u])
            continue;

        visited[u] = true;

        Edge* edge = graph->array[u];

        while (edge != NULL)
        {
            int v      = edge->destination;
            int weight = edge->weight;

            if (!visited[v] && weight < key[v])
            {
                key[v]    = weight;
                parent[v] = u;
                insert_pq_graph(&pq, v, key[v]);
            }

            edge = edge->next;
        }
    }

    /* Prim's requires a connected graph. Abort if any vertex was unreachable
     * rather than printing a partial result that could be mistaken for a
     * valid MST. */
    for (int i = 0; i < vertices; i++)
    {
        if (!visited[i])
        {
            fprintf(stderr,
                    "\nGraph is disconnected. "
                    "Prim's MST requires a connected graph.\n");
            free(parent);
            free(key);
            free(visited);
            return;
        }
    }

    int total = 0;

    printf("\n=====================================\n");
    printf("  Prim's Minimum Spanning Tree\n");
    printf("=====================================\n");
    printf("%-10s %-10s\n", "Edge", "Weight");
    printf("%-10s %-10s\n", "----", "------");

    for (int i = 0; i < vertices; i++)
    {
        if (parent[i] != -1)
        {
            printf("%-4d - %-4d %-10d\n", parent[i], i, key[i]);
            total += key[i];
        }
    }

    printf("=====================================\n");
    printf("%-10s %-10d\n", "Total", total);

    free(parent);
    free(key);
    free(visited);
}

void prim_mst_demo(void)
{
    int graph_capacity;
    int edges;
    int starting_node;
    weightedGraph* graph = NULL;

    while (1)
    {
        int graph_capacity_status =
            safe_input_int(&graph_capacity,
                           "\nenter the number of vertices in the graph, "
                           "(between 1 and 10), enter '-1' to exit : ",
                           1, 10);

        if (graph_capacity_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Prim's MST demo.....\n");
            return;
        }

        if (graph_capacity_status == 0)
            continue;

        graph = create_weightedGraph(graph_capacity);

        if (!graph)
        {
            fprintf(stderr, "\nMemory allocation failed\n");
            return;
        }

        break;
    }

    int max_edges = graph_capacity * (graph_capacity - 1) / 2;

    char edges_prompt[128];
    snprintf(edges_prompt, sizeof(edges_prompt),
             "\nenter number of edges (between 0 and %d), enter '-1' to exit : ",
             max_edges);

    while (1)
    {
        int edges_status = safe_input_int(&edges, edges_prompt, 0, max_edges);

        if (edges_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Prim's MST demo.....\n");
            free_weightedGraph(graph);
            return;
        }

        if (edges_status == 0)
            continue;

        break;
    }

    printf(
        "\nEnter source, destination, weight pairs (Source, Destination must be b/w 0 and %d "
        "(both inclusive)):\n",
        graph_capacity - 1);

    for (int i = 0; i < edges; i++)
    {
        int src;
        int dest;
        int wt;

        while (1)
        {
            int src_status = safe_input_int(&src, "src: ", 0, graph_capacity - 1);

            if (src_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Prim's MST demo.....\n");
                free_weightedGraph(graph);
                return;
            }

            if (src_status == 0)
                continue;

            break;
        }

        while (1)
        {
            int dest_status = safe_input_int(&dest, "dest: ", 0, graph_capacity - 1);

            if (dest_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Prim's MST demo.....\n");
                free_weightedGraph(graph);
                return;
            }

            if (dest_status == 0)
                continue;

            break;
        }

        while (1)
        {
            int wt_status = safe_input_int(&wt, "weight: ", 0, INT_MAX);

            if (wt_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting Prim's MST demo.....\n");
                free_weightedGraph(graph);
                return;
            }

            if (wt_status == 0)
                continue;

            break;
        }

        add_edge_directed(graph, src, dest, wt);
        add_edge_directed(graph, dest, src, wt);
    }

    while (1)
    {
        int start_status =
            safe_input_int(&starting_node,
                           "\nenter starting node: ",
                           0, graph_capacity - 1);

        if (start_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Prim's MST demo.....\n");
            free_weightedGraph(graph);
            return;
        }

        if (start_status == 0)
            continue;

        break;
    }

    printf("\n");
    prim_mst(graph, starting_node);
    free_weightedGraph(graph);
}