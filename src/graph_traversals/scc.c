#include "graph_traversals.h"
#include <stdlib.h>
#include <string.h>

static void tarjan_dfs(Graph* graph, int u, int* disc, int* low, bool* on_stack,
                       stack* st, int* time, int*** sccs, int** sizes, int* count)
{
    disc[u] = low[u] = ++(*time);
    push(st, u);
    on_stack[u] = true;

    Node* temp = graph->array[u];
    while (temp != NULL)
    {
        int v = temp->data;
        if (disc[v] == -1)
        {
            tarjan_dfs(graph, v, disc, low, on_stack, st, time, sccs, sizes, count);
            if (low[v] < low[u])
            {
                low[u] = low[v];
            }
        }
        else if (on_stack[v])
        {
            if (disc[v] < low[u])
            {
                low[u] = disc[v];
            }
        }
        temp = temp->next;
    }

    if (low[u] == disc[u])
    {
        int* comp = NULL;
        int comp_size = 0;
        int w = -1;
        while (w != u)
        {
            w = pop(st);
            on_stack[w] = false;
            comp_size++;
            int* new_comp = realloc(comp, sizeof(int) * comp_size);
            if (new_comp == NULL)
            {
                free(comp);
                return;
            }
            comp = new_comp;
            comp[comp_size - 1] = w;
        }

        (*count)++;
        int** new_sccs = realloc(*sccs, sizeof(int*) * (*count));
        int* new_sizes = realloc(*sizes, sizeof(int) * (*count));
        if (new_sccs == NULL || new_sizes == NULL)
        {
            free(comp);
            return;
        }
        *sccs = new_sccs;
        *sizes = new_sizes;
        (*sccs)[*count - 1] = comp;
        (*sizes)[*count - 1] = comp_size;
    }
}

int** find_scc_tarjan(Graph* graph, int* scc_count, int** scc_sizes)
{
    if (graph == NULL || scc_count == NULL || scc_sizes == NULL)
        return NULL;

    int V = graph->V;
    int* disc = malloc(sizeof(int) * V);
    int* low = malloc(sizeof(int) * V);
    bool* on_stack = malloc(sizeof(bool) * V);
    if (disc == NULL || low == NULL || on_stack == NULL)
    {
        free(disc);
        free(low);
        free(on_stack);
        return NULL;
    }

    for (int i = 0; i < V; i++)
    {
        disc[i] = -1;
        low[i] = -1;
        on_stack[i] = false;
    }

    stack* st = createStack();
    if (st == NULL)
    {
        free(disc);
        free(low);
        free(on_stack);
        return NULL;
    }

    int time = 0;
    int** sccs = NULL;
    int* sizes = NULL;
    int count = 0;

    for (int i = 0; i < V; i++)
    {
        if (disc[i] == -1)
        {
            tarjan_dfs(graph, i, disc, low, on_stack, st, &time, &sccs, &sizes, &count);
        }
    }

    destroyStack(st);
    free(disc);
    free(low);
    free(on_stack);

    *scc_count = count;
    *scc_sizes = sizes;
    return sccs;
}

static void kosaraju_dfs1(Graph* graph, int u, bool* visited, stack* st)
{
    visited[u] = true;
    Node* temp = graph->array[u];
    while (temp != NULL)
    {
        int v = temp->data;
        if (!visited[v])
        {
            kosaraju_dfs1(graph, v, visited, st);
        }
        temp = temp->next;
    }
    push(st, u);
}

static Graph* transpose_graph(Graph* graph)
{
    int V = graph->V;
    Graph* trans = create_graph(V);
    if (trans == NULL)
        return NULL;

    for (int u = 0; u < V; u++)
    {
        Node* temp = graph->array[u];
        while (temp != NULL)
        {
            int v = temp->data;
            add_edge_directed_unweighted(trans, v, u);
            temp = temp->next;
        }
    }
    return trans;
}

static void kosaraju_dfs2(Graph* graph, int u, bool* visited, int** comp, int* comp_size)
{
    visited[u] = true;
    (*comp_size)++;
    int* new_comp = realloc(*comp, sizeof(int) * (*comp_size));
    if (new_comp == NULL)
    {
        free(*comp);
        return;
    }
    *comp = new_comp;
    (*comp)[*comp_size - 1] = u;

    Node* temp = graph->array[u];
    while (temp != NULL)
    {
        int v = temp->data;
        if (!visited[v])
        {
            kosaraju_dfs2(graph, v, visited, comp, comp_size);
        }
        temp = temp->next;
    }
}

int** find_scc_kosaraju(Graph* graph, int* scc_count, int** scc_sizes)
{
    if (graph == NULL || scc_count == NULL || scc_sizes == NULL)
        return NULL;

    int V = graph->V;
    bool* visited = malloc(sizeof(bool) * V);
    if (visited == NULL)
        return NULL;
    memset(visited, 0, sizeof(bool) * V);

    stack* st = createStack();
    if (st == NULL)
    {
        free(visited);
        return NULL;
    }

    for (int i = 0; i < V; i++)
    {
        if (!visited[i])
        {
            kosaraju_dfs1(graph, i, visited, st);
        }
    }

    Graph* trans = transpose_graph(graph);
    if (trans == NULL)
    {
        destroyStack(st);
        free(visited);
        return NULL;
    }

    memset(visited, 0, sizeof(bool) * V);
    int** sccs = NULL;
    int* sizes = NULL;
    int count = 0;

    while (!isEmpty(st))
    {
        int u = pop(st);
        if (!visited[u])
        {
            int* comp = NULL;
            int comp_size = 0;
            kosaraju_dfs2(trans, u, visited, &comp, &comp_size);

            count++;
            int** new_sccs = realloc(sccs, sizeof(int*) * count);
            int* new_sizes = realloc(sizes, sizeof(int) * count);
            if (new_sccs == NULL || new_sizes == NULL)
            {
                free(comp);
                free_scc_result(sccs, sizes, count - 1);
                free_graph(trans);
                destroyStack(st);
                free(visited);
                return NULL;
            }
            sccs = new_sccs;
            sizes = new_sizes;
            sccs[count - 1] = comp;
            sizes[count - 1] = comp_size;
        }
    }

    free_graph(trans);
    destroyStack(st);
    free(visited);

    *scc_count = count;
    *scc_sizes = sizes;
    return sccs;
}

void free_scc_result(int** sccs, int* scc_sizes, int scc_count)
{
    if (sccs != NULL)
    {
        for (int i = 0; i < scc_count; i++)
        {
            if (sccs[i] != NULL)
            {
                free(sccs[i]);
            }
        }
        free(sccs);
    }
    if (scc_sizes != NULL)
    {
        free(scc_sizes);
    }
}
