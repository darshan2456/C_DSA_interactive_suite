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
                if (dist[i][k] != INF && dist[k][j] != INF && dist[i][k] + dist[k][j] < dist[i][j])
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
    printf("\n=== All-Pairs Shortest Paths (Floyd-Warshall) ===\n");
    printf("(INF = No Path Exists)\n");

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
