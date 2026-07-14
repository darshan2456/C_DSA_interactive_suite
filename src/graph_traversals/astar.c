#include "graph_traversals.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int astar_solve(weightedGraph* graph, int start, int dest, int h[], int parent[])
{
    if (graph == NULL || start < 0 || start >= graph->V || dest < 0 || dest >= graph->V ||
        h == NULL)
        return -1;

    int size = graph->V;
    int* visited = calloc(size, sizeof(int));
    int* dist = malloc(size * sizeof(int));
    int* fScore = malloc(size * sizeof(int));
    int result = INT_MAX;
    PQ_graph pq = {0};

    if (!visited || !dist || !fScore)
        goto cleanup;

    for (int i = 0; i < size; i++)
    {
        dist[i] = INT_MAX;
        fScore[i] = INT_MAX;
        if (parent)
        {
            parent[i] = -1;
        }
    }

    // Reuse the shared graph priority queue: a min-heap keyed on the node's
    // "distance" field, which here carries the f-score (f = g + h). Duplicate
    // entries are handled lazily via the visited[] check on pop.
    init_pq_graph(&pq, 10);

    if (!pq.heap)
    {
        result = -1;
        goto cleanup;
    }

    dist[start] = 0;
    fScore[start] = h[start];

    if (!insert_pq_graph(&pq, start, fScore[start]))
    {
        printf("Malloc failed\n");
        result = -1;
        goto cleanup;
    }

    PQ_graph_node popped;
    while (extractTop_pq_graph(&pq, &popped))
    {
        int u = popped.vertex;

        if (visited[u])
        {
            continue;
        }

        // Display expansion details for learning/trace (popped.distance == f)
        printf("[Expansion] Popped Node %d | g = %d, h = %d, f = %d\n", u, dist[u], h[u],
               popped.distance);

        // Goal timing: stop when popped
        if (u == dest)
        {
            result = dist[u];
            break;
        }

        visited[u] = 1;

        Edge* current = graph->array[u];
        while (current != NULL)
        {
            int v = current->destination;
            int currentWeight = current->weight;

            if (!visited[v] && dist[u] != INT_MAX)
            {
                int tentative_g = dist[u] + currentWeight;
                if (tentative_g < dist[v])
                {
                    dist[v] = tentative_g;
                    if (parent)
                    {
                        parent[v] = u;
                    }

                    int tentative_f = tentative_g + h[v];
                    if (tentative_f < 0)
                    {
                        tentative_f = INT_MAX;
                    }
                    fScore[v] = tentative_f;

                    if (!insert_pq_graph(&pq, v, fScore[v]))
                    {
                        printf("Malloc failed\n");
                        result = -1;
                        goto cleanup;
                    }
                }
            }
            current = current->next;
        }
    }

cleanup:
    PQ_Destroy(&pq);
    free(visited);
    free(dist);
    free(fScore);
    return result;
}

// note: the time measured by clock() covers the A* search computation only (the
// astar_solve call); it excludes path reconstruction and printing. it is for
// demonstration only and must not be treated as a measure of efficiency.
void astar(weightedGraph* graph, int start, int dest, int h[])
{
    int size = graph->V;
    int* parent = malloc(size * sizeof(int));
    if (!parent)
    {
        printf("Memory allocation failed in A*\n");
        return;
    }

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();
    int cost = astar_solve(graph, start, dest, h, parent);

    if (cost == -1)
    {
        printf("Astar failed");
        return;
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    printf("\ntotal CPU time taken for A* search:- %f seconds\n", total_t);

    if (cost == INT_MAX || cost < 0)
    {
        printf("No path exists from %d to %d\n", start, dest);
    }
    else
    {
        int* path = malloc(size * sizeof(int));
        if (!path)
        {
            printf("Memory allocation failed in A*\n");
            free(parent);
            return;
        }
        int pathLen = 0;
        int curr = dest;
        while (curr != -1)
        {
            path[pathLen++] = curr;
            curr = parent[curr];
        }

        printf("Shortest Path: ");
        for (int i = pathLen - 1; i >= 0; i--)
        {
            printf("%d", path[i]);
            if (i > 0)
            {
                printf(" -> ");
            }
        }
        printf("\nTotal Cost: %d\n", cost);
        free(path);
    }
    free(parent);
}
