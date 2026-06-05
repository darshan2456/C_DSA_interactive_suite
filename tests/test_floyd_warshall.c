#include "graph_traversals.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define INF 9999999

// Helper to allocate dynamic matrix
static int** allocate_matrix(int V)
{
    int** matrix = malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++)
    {
        matrix[i] = malloc(V * sizeof(int));
    }
    return matrix;
}

// Helper to free dynamic matrix
static void free_matrix(int** matrix, int V)
{
    for (int i = 0; i < V; i++)
    {
        free(matrix[i]);
    }
    free(matrix);
}

void test_floyd_warshall_standard()
{
    int V = 4;
    int** graph = allocate_matrix(V);
    int** dist = allocate_matrix(V);
    int** next = allocate_matrix(V);

    /*
       0 -> 3 (weight 10)
       0 -> 1 (weight 3)
       1 -> 2 (weight 1)
       2 -> 3 (weight 2)
    */
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }
    graph[0][3] = 10;
    graph[0][1] = 3;
    graph[1][2] = 1;
    graph[2][3] = 2;

    floyd_warshall(graph, V, dist, next);

    // Shortest path from 0 to 3 should be 0 -> 1 -> 2 -> 3 with weight 3 + 1 + 2 = 6
    assert(dist[0][3] == 6);
    assert(next[0][3] == 1);
    assert(next[1][3] == 2);
    assert(next[2][3] == 3);

    // Other shortest paths
    assert(dist[0][2] == 4);
    assert(dist[1][3] == 3);
    assert(dist[3][0] == INF);

    free_matrix(graph, V);
    free_matrix(dist, V);
    free_matrix(next, V);

    printf("Floyd-Warshall standard graph test passed\n");
}

void test_floyd_warshall_negative_weights()
{
    int V = 3;
    int** graph = allocate_matrix(V);
    int** dist = allocate_matrix(V);
    int** next = allocate_matrix(V);

    /*
       0 -> 1 (weight 4)
       0 -> 2 (weight 8)
       1 -> 2 (weight -3)
    */
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }
    graph[0][1] = 4;
    graph[0][2] = 8;
    graph[1][2] = -3;

    floyd_warshall(graph, V, dist, next);

    // Shortest path from 0 to 2 should go 0 -> 1 -> 2 with weight 4 - 3 = 1
    assert(dist[0][2] == 1);
    assert(next[0][2] == 1);
    assert(next[1][2] == 2);

    free_matrix(graph, V);
    free_matrix(dist, V);
    free_matrix(next, V);

    printf("Floyd-Warshall negative weights test passed\n");
}

void test_floyd_warshall_disconnected()
{
    int V = 3;
    int** graph = allocate_matrix(V);
    int** dist = allocate_matrix(V);
    int** next = allocate_matrix(V);

    // 0 and 1 are connected, 2 is completely isolated
    for (int i = 0; i < V; i++)
    {
        for (int j = 0; j < V; j++)
        {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }
    graph[0][1] = 5;

    floyd_warshall(graph, V, dist, next);

    assert(dist[0][1] == 5);
    assert(dist[0][2] == INF);
    assert(dist[1][2] == INF);
    assert(next[0][2] == -1);

    free_matrix(graph, V);
    free_matrix(dist, V);
    free_matrix(next, V);

    printf("Floyd-Warshall disconnected graph test passed\n");
}

int main()
{
    test_floyd_warshall_standard();
    test_floyd_warshall_negative_weights();
    test_floyd_warshall_disconnected();

    printf("All Floyd-Warshall tests passed\n");
    return 0;
}
