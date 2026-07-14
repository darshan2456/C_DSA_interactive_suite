#include "graph_traversals.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <time.h>

// note: the time measured by clock() covers the shortest-path computation,
// including the negative-cycle check, and stops before the distance table is
// printed. it is for demonstration only and not a measure of efficiency.
void bellman_ford(weightedGraph* graph, int start)
{
    int size = graph->V;

    int dist[size];

    for (int i = 0; i < size; i++)
        dist[i] = INT_MAX;

    dist[start] = 0;

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();

    for (int count = 0; count < size - 1; count++)
    {
        for (int u = 0; u < size; u++)
        {
            Edge* current = graph->array[u];

            while (current != NULL)
            {
                int v = current->destination;
                int weight = current->weight;

                if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                    dist[v] = dist[u] + weight;

                current = current->next;
            }
        }
    }

    // Negative edge cycle detector
    for (int u = 0; u < size; u++)
    {
        Edge* current = graph->array[u];
        while (current != NULL)
        {
            int v = current->destination;
            int weight = current->weight;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
            {
                end_t = clock();
                total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;
                printf("Negative edge cycle detected. Graph is not suitable for bellman ford.\n");
                printf("\ntotal CPU time taken for Bellman-Ford:- %f seconds\n", total_t);
                return;
            }

            current = current->next;
        }
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    // Print vertex, distance stuff
    printf("Start -> Vertex  \t  Distance\n");
    printf("---------------  \t  --------\n");

    for (int i = 0; i < size; i++)
    {
        if (dist[i] == INT_MAX)
            printf("    %d -> %d  \t            INF   \n", start, i);
        else
            printf("    %d -> %d  \t             %d   \n", start, i, dist[i]);
    }

    printf("\ntotal CPU time taken for Bellman-Ford:- %f seconds\n", total_t);
}
