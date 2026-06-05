#include "graph_traversals.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define INF 9999999 // Represent infinity (safe from overflow)

/**
 * Runs the Floyd-Warshall all-pairs shortest path algorithm.
 * 
 * @param graph Dynamic 2D array representing adjacency matrix of size V x V
 * @param V Number of vertices
 * @param dist Output distance matrix (already allocated V x V)
 * @param next Output successor matrix for path reconstruction (already allocated V x V)
 */
void floyd_warshall(int** graph, int V, int** dist, int** next)
{
    // 1. Initialize dist and next matrices
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            dist[i][j] = graph[i][j];
            if (graph[i][j] != INF && i != j)
            {
                next[i][j] = j;
            }
            else
            {
                next[i][j] = -1;
            }
        }
    }

    // 2. Floyd-Warshall main dynamic programming loop
    for (int k = 0; k < V; k++)
    {
        for (int i = 0; i < V; i++)
        {
            for (int j = 0; j < V; j++)
            {
                // Check if paths through vertex k are shorter
                if (dist[i][k] != INF && dist[k][j] != INF &&
                    dist[i][k] + dist[k][j] < dist[i][j])
                {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

/**
 * Prints the results of the Floyd-Warshall algorithm, including the distance matrix,
 * next/path matrix, and the reconstructed shortest paths between all pairs of nodes.
 */
void print_floyd_warshall_solution(int** dist, int** next, int V)
{
    printf("\nDistance Matrix:\n");
    printf("     ");
    for (int i = 0; i < V; i++)
    {
        printf("%5d ", i);
    }
    printf("\n");
    
    for (int i = 0; i < V; i++)
    {
        printf("%3d: ", i);
        for (int j = 0; j < V; j++)
        {
            if (dist[i][j] == INF)
            {
                printf("  INF ");
            }
            else
            {
                printf("%5d ", dist[i][j]);
            }
        }
        printf("\n");
    }

    printf("\nReconstructed Shortest Paths:\n");
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            if (i == j)
            {
                printf("  %d -> %d: Distance = 0, Path = [%d]\n", i, j, i);
                continue;
            }
            if (dist[i][j] == INF)
            {
                printf("  %d -> %d: No path exists (Distance = INF)\n", i, j);
                continue;
            }

            printf("  %d -> %d: Distance = %d, Path = [%d", i, j, dist[i][j], i);
            int curr = i;
            while (curr != j)
            {
                curr = next[curr][j];
                printf(" -> %d", curr);
            }
            printf("]\n");
        }
    }
}

/**
 * Interactive demo for testing the Floyd-Warshall algorithm.
 */
void floyd_warshall_demo(void)
{
    int vertices = 0;
    int edges = 0;

    // Get number of vertices
    while (1)
    {
        int status = safe_input_int(&vertices,
                                    "\nEnter the number of vertices in the graph (between 1 and 10), or enter '-1' to exit: ",
                                    1, 10);
        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Floyd-Warshall demo...\n");
            return;
        }
        if (status == 0)
        {
            continue;
        }
        break;
    }

    // Get number of edges
    while (1)
    {
        int max_edges = vertices * (vertices - 1);
        int status = safe_input_int(&edges,
                                    "\nEnter the number of edges, or enter '-1' to exit: ",
                                    0, max_edges);
        if (status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Floyd-Warshall demo...\n");
            return;
        }
        if (status == 0)
        {
            continue;
        }
        break;
    }

    // Dynamically allocate graph, dist, and next matrices
    int** graph = malloc(vertices * sizeof(int*));
    int** dist = malloc(vertices * sizeof(int*));
    int** next = malloc(vertices * sizeof(int*));
    if (!graph || !dist || !next)
    {
        printf("\nMemory allocation failed.\n");
        // Safe cleanup if allocation partially fails
        if (graph) free(graph);
        if (dist) free(dist);
        if (next) free(next);
        return;
    }

    for (int i = 0; i < vertices; i++)
    {
        graph[i] = malloc(vertices * sizeof(int));
        dist[i] = malloc(vertices * sizeof(int));
        next[i] = malloc(vertices * sizeof(int));
        if (!graph[i] || !dist[i] || !next[i])
        {
            printf("\nMemory allocation failed.\n");
            // Free everything allocated so far
            for (int j = 0; j <= i; j++)
            {
                if (graph[j]) free(graph[j]);
                if (dist[j]) free(dist[j]);
                if (next[j]) free(next[j]);
            }
            free(graph);
            free(dist);
            free(next);
            return;
        }

        // Initialize adjacency matrix values
        for (int j = 0; j < vertices; j++)
        {
            if (i == j)
            {
                graph[i][j] = 0;
            }
            else
            {
                graph[i][j] = INF;
            }
        }
    }

    if (edges > 0)
    {
        printf("\nEnter source, destination, and weight for each edge:\n");
        printf("(Vertices must be between 0 and %d. Weights can be negative, but avoid negative cycles)\n", vertices - 1);
    }

    for (int i = 0; i < edges; i++)
    {
        int src = 0, dest = 0, weight = 0;
        int status;

    retry_src:
        status = safe_input_int(&src, "Source node: ", 0, vertices - 1);
        if (status == INPUT_EXIT_SIGNAL)
        {
            goto cleanup;
        }
        if (status == 0)
        {
            goto retry_src;
        }

    retry_dest:
        status = safe_input_int(&dest, "Destination node: ", 0, vertices - 1);
        if (status == INPUT_EXIT_SIGNAL)
        {
            goto cleanup;
        }
        if (status == 0)
        {
            goto retry_dest;
        }

    retry_weight:
        // Accept negative weights but set reasonable bounds to prevent overflows
        status = safe_input_int(&weight, "Edge weight (e.g. -1000 to 1000): ", -1000, 1000);
        if (status == INPUT_EXIT_SIGNAL)
        {
            goto cleanup;
        }
        if (status == 0)
        {
            goto retry_weight;
        }

        graph[src][dest] = weight;
    }

    // Run the algorithm
    floyd_warshall(graph, vertices, dist, next);

    // Detect negative weight cycles
    int has_negative_cycle = 0;
    for (int i = 0; i < vertices; i++)
    {
        if (dist[i][i] < 0)
        {
            has_negative_cycle = 1;
            break;
        }
    }

    if (has_negative_cycle)
    {
        printf("\nWarning: The graph contains a negative weight cycle. Shortest paths are undefined.\n");
    }
    else
    {
        print_floyd_warshall_solution(dist, next, vertices);
    }

cleanup:
    // Free dynamic allocations
    for (int i = 0; i < vertices; i++)
    {
        free(graph[i]);
        free(dist[i]);
        free(next[i]);
    }
    free(graph);
    free(dist);
    free(next);
}
