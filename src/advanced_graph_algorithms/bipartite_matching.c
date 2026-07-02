#include "advanced_graph_algorithms.h"
#include "display_header.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool bipartite_color(Graph* graph, int* color)
{
    int V = graph->V;
    for (int i = 0; i < V; i++)
        color[i] = -1;

    int* queue = malloc(sizeof(int) * V);
    if (queue == NULL)
        return false;

    for (int start = 0; start < V; start++)
    {
        if (color[start] == -1)
        {
            int head = 0, tail = 0;
            queue[tail++] = start;
            color[start] = 0;

            while (head < tail)
            {
                int u = queue[head++];
                Node* temp = graph->array[u];
                while (temp != NULL)
                {
                    int v = temp->data;
                    if (color[v] == -1)
                    {
                        color[v] = 1 - color[u];
                        queue[tail++] = v;
                    }
                    else if (color[v] == color[u])
                    {
                        free(queue);
                        return false;
                    }
                    temp = temp->next;
                }
            }
        }
    }

    free(queue);
    return true;
}

static bool bfs_dinic_matching(int V, int** residual, int source, int sink, int* level)
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

static int dfs_dinic_matching(int V, int** residual, int u, int sink, int flow, int* level,
                              int* start)
{
    if (u == sink)
        return flow;

    for (; start[u] < V; start[u]++)
    {
        int v = start[u];
        if (level[v] == level[u] + 1 && residual[u][v] > 0)
        {
            int curr_flow = flow < residual[u][v] ? flow : residual[u][v];
            int temp_flow = dfs_dinic_matching(V, residual, v, sink, curr_flow, level, start);
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

int max_bipartite_matching(Graph* graph, int** match_pairs, int* match_count)
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

    int source = V;
    int sink = V + 1;
    int total_vertices = V + 2;

    int** residual = malloc(sizeof(int*) * total_vertices);
    if (residual == NULL)
    {
        free(color);
        return 0;
    }
    for (int i = 0; i < total_vertices; i++)
    {
        residual[i] = calloc(total_vertices, sizeof(int));
        if (residual[i] == NULL)
        {
            for (int j = 0; j < i; j++)
                free(residual[j]);
            free(residual);
            free(color);
            return 0;
        }
    }

    for (int u = 0; u < V; u++)
    {
        if (color[u] == 0)
        {
            residual[source][u] = 1;
            Node* temp = graph->array[u];
            while (temp != NULL)
            {
                int v = temp->data;
                if (color[v] == 1)
                {
                    residual[u][v] = 1;
                }
                temp = temp->next;
            }
        }
        else
        {
            residual[u][sink] = 1;
        }
    }

    int* level = malloc(sizeof(int) * total_vertices);
    int* start = malloc(sizeof(int) * total_vertices);
    if (level == NULL || start == NULL)
    {
        free(level);
        free(start);
        for (int i = 0; i < total_vertices; i++)
            free(residual[i]);
        free(residual);
        free(color);
        return 0;
    }

    int max_flow = 0;

    while (bfs_dinic_matching(total_vertices, residual, source, sink, level))
    {
        memset(start, 0, sizeof(int) * total_vertices);
        while (1)
        {
            int flow =
                dfs_dinic_matching(total_vertices, residual, source, sink, INT_MAX, level, start);
            if (flow == 0)
                break;
            max_flow += flow;
        }
    }

    // Now extract matches from residual matrix
    // If color[u] == 0 and color[v] == 1, and residual[u][v] == 0,
    // but originally we set residual[u][v] to 1, then edge u -> v is matched!
    // Since we don't store original capacity separately, we can check the original graph
    int count = 0;
    int* pairs = NULL;

    for (int u = 0; u < V; u++)
    {
        if (color[u] == 0)
        {
            Node* temp = graph->array[u];
            while (temp != NULL)
            {
                int v = temp->data;
                if (color[v] == 1 && residual[u][v] == 0)
                {
                    count++;
                    int* new_pairs = realloc(pairs, sizeof(int) * 2 * count);
                    if (new_pairs == NULL)
                    {
                        free(pairs);
                        free(level);
                        free(start);
                        for (int i = 0; i < total_vertices; i++)
                            free(residual[i]);
                        free(residual);
                        free(color);
                        return 0;
                    }
                    pairs = new_pairs;
                    pairs[2 * (count - 1)] = u;
                    pairs[2 * (count - 1) + 1] = v;
                }
                temp = temp->next;
            }
        }
    }

    (void)max_flow;
    free(level);
    free(start);
    for (int i = 0; i < total_vertices; i++)
        free(residual[i]);
    free(residual);
    free(color);

    *match_count = count;
    *match_pairs = pairs;
    return count;
}

void bipartite_matching_demo(void)
{
    int vertices, edges;
    Graph* graph = NULL;

    printf("\nMaximum Bipartite Matching Demo\n");
    int input_status = safe_input_int(&vertices, "Enter number of vertices (2-20): ", 2, 20);
    if (input_status == INPUT_EXIT_SIGNAL)
        return;

    graph = create_graph(vertices);
    int edge_status = safe_input_int(&edges, "Enter number of edges (1-100): ", 1, 100);
    if (edge_status == INPUT_EXIT_SIGNAL)
    {
        free_graph(graph);
        return;
    }

    printf("Enter undirected edges (u v) with vertices from 0 to %d:\n", vertices - 1);
    for (int i = 0; i < edges; i++)
    {
        int u, v;
        if (safe_input_int(&u, "u: ", 0, vertices - 1) == INPUT_EXIT_SIGNAL)
        {
            free_graph(graph);
            return;
        }
        if (safe_input_int(&v, "v: ", 0, vertices - 1) == INPUT_EXIT_SIGNAL)
        {
            free_graph(graph);
            return;
        }
        add_edge_undirected(graph, u, v);
    }

    int match_count_dinic = 0;
    int* match_pairs_dinic = NULL;
    int max_dinic = max_bipartite_matching(graph, &match_pairs_dinic, &match_count_dinic);

    int match_count_hk = 0;
    int* match_pairs_hk = NULL;
    int max_hk = hopcroft_karp(graph, &match_pairs_hk, &match_count_hk);

    printf("\n=== Bipartite Matching Results ===\n");
    printf("Max Matching (Dinic's Flow Network): %d\n", max_dinic);
    if (match_count_dinic > 0)
    {
        printf("Matching Pairs (Dinic's): ");
        for (int i = 0; i < match_count_dinic; i++)
        {
            printf("(%d - %d) ", match_pairs_dinic[2 * i], match_pairs_dinic[2 * i + 1]);
        }
        printf("\n");
        free(match_pairs_dinic);
    }

    printf("Max Matching (Hopcroft-Karp): %d\n", max_hk);
    if (match_count_hk > 0)
    {
        printf("Matching Pairs (Hopcroft-Karp): ");
        for (int i = 0; i < match_count_hk; i++)
        {
            printf("(%d - %d) ", match_pairs_hk[2 * i], match_pairs_hk[2 * i + 1]);
        }
        printf("\n");
        free(match_pairs_hk);
    }

    free_graph(graph);
    printf("\nPress Enter to continue...");
    getchar();
}
