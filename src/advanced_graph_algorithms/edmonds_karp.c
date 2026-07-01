#include "advanced_graph_algorithms.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static bool bfs_augmenting_path(int V, int** residual, int source, int sink, int* parent)
{
    bool* visited = calloc(V, sizeof(bool));
    if (visited == NULL)
        return false;

    int* queue = malloc(sizeof(int) * V);
    if (queue == NULL)
    {
        free(visited);
        return false;
    }
    int head = 0, tail = 0;

    queue[tail++] = source;
    visited[source] = true;

    while (head < tail)
    {
        int u = queue[head++];
        if (u == sink)
        {
            free(visited);
            free(queue);
            return true;
        }

        for (int v = 0; v < V; v++)
        {
            if (!visited[v] && residual[u][v] > 0)
            {
                parent[v] = u;
                visited[v] = true;
                queue[tail++] = v;
            }
        }
    }

    free(visited);
    free(queue);
    return false;
}

int edmonds_karp(weightedGraph* graph, int source, int sink)
{
    if (graph == NULL || source < 0 || source >= graph->V || sink < 0 || sink >= graph->V)
        return 0;

    int V = graph->V;
    int** residual = malloc(sizeof(int*) * V);
    if (residual == NULL)
        return 0;

    for (int i = 0; i < V; i++)
    {
        residual[i] = calloc(V, sizeof(int));
        if (residual[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(residual[j]);
            free(residual);
            return 0;
        }
    }

    for (int u = 0; u < V; u++)
    {
        Edge* temp = graph->array[u];
        while (temp != NULL)
        {
            residual[u][temp->destination] = temp->weight;
            temp = temp->next;
        }
    }

    int* parent = malloc(sizeof(int) * V);
    if (parent == NULL)
    {
        for (int i = 0; i < V; i++)
            free(residual[i]);
        free(residual);
        return 0;
    }

    int max_flow = 0;

    while (1)
    {
        for (int i = 0; i < V; i++)
            parent[i] = -1;

        if (!bfs_augmenting_path(V, residual, source, sink, parent))
        {
            break;
        }

        int path_flow = INT_MAX;
        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            if (residual[u][v] < path_flow)
            {
                path_flow = residual[u][v];
            }
        }

        for (int v = sink; v != source; v = parent[v])
        {
            int u = parent[v];
            residual[u][v] -= path_flow;
            residual[v][u] += path_flow;
        }

        max_flow += path_flow;
    }

    for (int i = 0; i < V; i++)
    {
        free(residual[i]);
    }
    free(residual);
    free(parent);

    return max_flow;
}
