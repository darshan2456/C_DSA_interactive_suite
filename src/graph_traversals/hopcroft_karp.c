#include "graph_traversals.h"
#include <limits.h>
#include <stdlib.h>
#include <string.h>

static bool hopcroft_karp_bfs(Graph* graph, int* pair_u, int* pair_v, int* dist, int V, int* color)
{
    int* queue = malloc(sizeof(int) * V);
    if (queue == NULL)
        return false;
    int head = 0, tail = 0;

    for (int u = 0; u < V; u++)
    {
        if (color[u] == 0)
        {
            if (pair_u[u] == V)
            {
                dist[u] = 0;
                queue[tail++] = u;
            }
            else
            {
                dist[u] = INT_MAX;
            }
        }
    }
    dist[V] = INT_MAX;

    while (head < tail)
    {
        int u = queue[head++];
        if (dist[u] < dist[V])
        {
            Node* temp = graph->array[u];
            while (temp != NULL)
            {
                int v = temp->data;
                if (color[v] == 1)
                {
                    if (dist[pair_v[v]] == INT_MAX)
                    {
                        dist[pair_v[v]] = dist[u] + 1;
                        queue[tail++] = pair_v[v];
                    }
                }
                temp = temp->next;
            }
        }
    }

    bool has_path = (dist[V] != INT_MAX);
    free(queue);
    return has_path;
}

static bool hopcroft_karp_dfs(Graph* graph, int u, int* pair_u, int* pair_v, int* dist, int V,
                              int* color)
{
    if (u != V)
    {
        Node* temp = graph->array[u];
        while (temp != NULL)
        {
            int v = temp->data;
            if (color[v] == 1)
            {
                if (dist[pair_v[v]] == dist[u] + 1)
                {
                    if (hopcroft_karp_dfs(graph, pair_v[v], pair_u, pair_v, dist, V, color))
                    {
                        pair_v[v] = u;
                        pair_u[u] = v;
                        return true;
                    }
                }
            }
            temp = temp->next;
        }
        dist[u] = INT_MAX;
        return false;
    }
    return true;
}

int hopcroft_karp(Graph* graph, int** match_pairs, int* match_count)
{
    if (graph == NULL || match_pairs == NULL || match_count == NULL)
        return 0;

    int V = graph->V;
    int* color = malloc(sizeof(int) * V);
    if (color == NULL)
        return 0;

    if (!bipartite_color(graph, color))
    {
        free(color);
        *match_count = 0;
        *match_pairs = NULL;
        return 0;
    }

    int* pair_u = malloc(sizeof(int) * (V + 1));
    int* pair_v = malloc(sizeof(int) * (V + 1));
    int* dist = malloc(sizeof(int) * (V + 1));
    if (pair_u == NULL || pair_v == NULL || dist == NULL)
    {
        free(color);
        free(pair_u);
        free(pair_v);
        free(dist);
        return 0;
    }

    for (int i = 0; i <= V; i++)
    {
        pair_u[i] = V;
        pair_v[i] = V;
    }

    int matching = 0;
    while (hopcroft_karp_bfs(graph, pair_u, pair_v, dist, V, color))
    {
        for (int u = 0; u < V; u++)
        {
            if (color[u] == 0 && pair_u[u] == V)
            {
                if (hopcroft_karp_dfs(graph, u, pair_u, pair_v, dist, V, color))
                {
                    matching++;
                }
            }
        }
    }

    int count = 0;
    int* pairs = malloc(sizeof(int) * 2 * matching);
    if (pairs == NULL && matching > 0)
    {
        free(color);
        free(pair_u);
        free(pair_v);
        free(dist);
        return 0;
    }

    for (int u = 0; u < V; u++)
    {
        if (color[u] == 0 && pair_u[u] != V)
        {
            pairs[2 * count] = u;
            pairs[2 * count + 1] = pair_u[u];
            count++;
        }
    }

    free(color);
    free(pair_u);
    free(pair_v);
    free(dist);

    *match_count = count;
    *match_pairs = pairs;
    return count;
}
