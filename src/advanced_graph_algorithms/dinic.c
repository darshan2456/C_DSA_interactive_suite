#include "advanced_graph_algorithms.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static bool dinic_bfs(int V, int** residual, int source, int sink, int* level)
{
    for (int i = 0; i < V; i++)
        level[i] = -1;
    level[source] = 0;

    int* queue = malloc(sizeof(int) * V);
    if (queue == NULL)
        return false;
    int head = 0, tail = 0;
    queue[tail++] = source;

    while (head < tail)
    {
        int u = queue[head++];
        for (int v = 0; v < V; v++)
        {
            if (level[v] < 0 && residual[u][v] > 0)
            {
                level[v] = level[u] + 1;
                queue[tail++] = v;
            }
        }
    }

    bool reachable = (level[sink] >= 0);
    free(queue);
    return reachable;
}

static int dinic_dfs(int V, int** residual, int u, int sink, int flow, int* level, int* start)
{
    if (u == sink)
        return flow;

    for (; start[u] < V; start[u]++)
    {
        int v = start[u];
        if (level[v] == level[u] + 1 && residual[u][v] > 0)
        {
            int curr_flow = flow < residual[u][v] ? flow : residual[u][v];
            int temp_flow = dinic_dfs(V, residual, v, sink, curr_flow, level, start);
            if (temp_flow > 0)
            {
                residual[u][v] -= temp_flow;
                residual[v][u] += temp_flow;
                return temp_flow;
            }
        }
    }
    return 0;
}

int dinic(weightedGraph* graph, int source, int sink)
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

    int* level = malloc(sizeof(int) * V);
    int* start = malloc(sizeof(int) * V);
    if (level == NULL || start == NULL)
    {
        free(level);
        free(start);
        for (int i = 0; i < V; i++)
            free(residual[i]);
        free(residual);
        return 0;
    }

    int max_flow = 0;

    while (dinic_bfs(V, residual, source, sink, level))
    {
        memset(start, 0, sizeof(int) * V);
        while (1)
        {
            int flow = dinic_dfs(V, residual, source, sink, INT_MAX, level, start);
            if (flow == 0)
                break;
            max_flow += flow;
        }
    }

    for (int i = 0; i < V; i++)
    {
        free(residual[i]);
    }
    free(residual);
    free(level);
    free(start);

    return max_flow;
}
