#include "advanced_graph_algorithms.h"
#include "clear_screen.h"
#include "config.h"
#include "safe_input.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_graph_state(Graph* graph, int active_node, int* disc, int* low, bool* on_stack,
                              stack* st, const char* alg_name)
{
    clear_screen();
    printf("\n=== Strongly Connected Components Visualizer (%s) ===\n\n", alg_name);

    // Print graph adjacency list with active node highlighted
    printf("Graph Adjacency List:\n");
    for (int i = 0; i < graph->V; i++)
    {
        if (i == active_node)
        {
            printf("\033[1;31m-> [%d]:\033[0m ", i); // Red highlight for active node
        }
        else
        {
            printf("   [%d]: ", i);
        }

        Node* temp = graph->array[i];
        if (temp == NULL)
        {
            printf("NULL");
        }
        while (temp != NULL)
        {
            int val = (int)(intptr_t)temp->data;
            if (val == active_node)
            {
                printf("\033[1;33m%d\033[0m -> ", val); // Yellow for reference
            }
            else
            {
                printf("%d -> ", val);
            }
            temp = temp->next;
        }
        printf("end\n");
    }
    printf("\n");

    // Print values table
    if (disc != NULL && low != NULL)
    {
        printf("Node state table:\n");
        printf("Node | Disc Time | Low Link | On Stack\n");
        printf("-----+-----------+----------+----------\n");
        for (int i = 0; i < graph->V; i++)
        {
            char disc_str[16] = "-";
            char low_str[16] = "-";
            if (disc[i] != -1)
                snprintf(disc_str, sizeof(disc_str), "%d", disc[i]);
            if (low[i] != -1)
                snprintf(low_str, sizeof(low_str), "%d", low[i]);

            if (i == active_node)
            {
                printf("\033[1;31m %2d  |    %5s  |    %5s |   %5s\033[0m\n", i, disc_str, low_str,
                       on_stack[i] ? "Yes" : "No");
            }
            else
            {
                printf(" %2d  |    %5s  |    %5s |   %5s\n", i, disc_str, low_str,
                       on_stack[i] ? "Yes" : "No");
            }
        }
        printf("\n");
    }

    // Print Stack state
    if (st != NULL)
    {
        printf("Active Stack: ");
        printStackAsInts(st);
        printf("\n");
    }

    fflush(stdout);
    dynamic_sleep();
}

static void tarjan_dfs_vis(Graph* graph, int u, int* disc, int* low, bool* on_stack, stack* st,
                           int* time, int*** sccs, int** sizes, int* count, bool* success)
{
    if (!*success)
        return;

    disc[u] = low[u] = ++(*time);
    push(st, (void*)(intptr_t)u);
    on_stack[u] = true;

    print_graph_state(graph, u, disc, low, on_stack, st, "Tarjan DFS");

    Node* temp = graph->array[u];
    while (temp != NULL)
    {
        int v = (int)(intptr_t)temp->data;
        if (disc[v] == -1)
        {
            tarjan_dfs_vis(graph, v, disc, low, on_stack, st, time, sccs, sizes, count, success);
            if (!*success)
                return;
            if (low[v] < low[u])
            {
                low[u] = low[v];
                print_graph_state(graph, u, disc, low, on_stack, st, "Tarjan (Updated Low Link)");
            }
        }
        else if (on_stack[v])
        {
            if (disc[v] < low[u])
            {
                low[u] = disc[v];
                print_graph_state(graph, u, disc, low, on_stack, st, "Tarjan (Back Edge Low Link)");
            }
        }
        temp = temp->next;
    }

    if (low[u] == disc[u])
    {
        int* comp = NULL;
        int comp_size = 0;
        int w = -1;

        printf("\033[1;32mFound Strongly Connected Component (Root node %d):\033[0m ", u);

        while (w != u)
        {
            w = (int)(intptr_t)pop(st);
            on_stack[w] = false;
            comp_size++;
            int* new_comp = realloc(comp, sizeof(int) * comp_size);
            if (new_comp == NULL)
            {
                free(comp);
                *success = false;
                return;
            }
            comp = new_comp;
            comp[comp_size - 1] = w;
            printf("%d ", w);
        }
        printf("\n");
        fflush(stdout);
        dynamic_sleep();

        (*count)++;
        int** new_sccs = realloc(*sccs, sizeof(int*) * (*count));
        if (new_sccs == NULL)
        {
            free(comp);
            *success = false;
            return;
        }
        *sccs = new_sccs;

        int* new_sizes = realloc(*sizes, sizeof(int) * (*count));
        if (new_sizes == NULL)
        {
            free(comp);
            *success = false;
            return;
        }
        *sizes = new_sizes;
        (*sccs)[*count - 1] = comp;
        (*sizes)[*count - 1] = comp_size;
    }
}

void visualize_tarjan(Graph* graph)
{
    int V = graph->V;
    int* disc = malloc(sizeof(int) * V);
    int* low = malloc(sizeof(int) * V);
    bool* on_stack = malloc(sizeof(bool) * V);
    if (disc == NULL || low == NULL || on_stack == NULL)
    {
        free(disc);
        free(low);
        free(on_stack);
        return;
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
        return;
    }
    int time = 0;
    int** sccs = NULL;
    int* sizes = NULL;
    int count = 0;
    bool success = true;

    for (int i = 0; i < V; i++)
    {
        if (disc[i] == -1)
        {
            tarjan_dfs_vis(graph, i, disc, low, on_stack, st, &time, &sccs, &sizes, &count,
                           &success);
            if (!success)
            {
                free_scc_result(sccs, sizes, count);
                destroyStack(st, NULL);
                free(disc);
                free(low);
                free(on_stack);
                return;
            }
        }
    }

    clear_screen();
    printf("\n=== Tarjan's SCC Algorithm Final Result ===\n\n");
    printf("Total components found: %d\n", count);
    for (int i = 0; i < count; i++)
    {
        printf("Component %d: ", i + 1);
        for (int j = 0; j < sizes[i]; j++)
        {
            printf("%d ", sccs[i][j]);
        }
        printf("\n");
    }
    printf("\nPress Enter to continue...");
    fflush(stdout);
    getchar();

    destroyStack(st, NULL);
    free(disc);
    free(low);
    free(on_stack);
    free_scc_result(sccs, sizes, count);
}

static void kosaraju_dfs1_vis(Graph* graph, int u, bool* visited, stack* st, const char* phase)
{
    visited[u] = true;
    clear_screen();
    printf("\n=== Kosaraju's Phase 1 DFS (%s) ===\n\n", phase);
    printf("Visited Node: %d\n", u);
    printf("Finished Stack: ");
    printStackAsInts(st);
    printf("\n");
    dynamic_sleep();

    Node* temp = graph->array[u];
    while (temp != NULL)
    {
        int v = (int)(intptr_t)temp->data;
        if (!visited[v])
        {
            kosaraju_dfs1_vis(graph, v, visited, st, phase);
        }
        temp = temp->next;
    }
    push(st, (void*)(intptr_t)u);
}

static void kosaraju_dfs2_vis(Graph* graph, int u, bool* visited, int** comp, int* comp_size)
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

    clear_screen();
    printf("\n=== Kosaraju's Phase 2 DFS (Transposed Graph SCC) ===\n\n");
    printf("Current SCC component so far: ");
    for (int i = 0; i < *comp_size; i++)
    {
        printf("%d ", (*comp)[i]);
    }
    printf("\n");
    dynamic_sleep();

    Node* temp = graph->array[u];
    while (temp != NULL)
    {
        int v = (int)(intptr_t)temp->data;
        if (!visited[v])
        {
            kosaraju_dfs2_vis(graph, v, visited, comp, comp_size);
        }
        temp = temp->next;
    }
}

void visualize_kosaraju(Graph* graph)
{
    int V = graph->V;
    bool* visited = malloc(sizeof(bool) * V);
    memset(visited, 0, sizeof(bool) * V);

    stack* st = createStack();
    for (int i = 0; i < V; i++)
    {
        if (!visited[i])
        {
            kosaraju_dfs1_vis(graph, i, visited, st, "Original Graph DFS");
        }
    }

    Graph* trans = transpose_graph(graph);
    memset(visited, 0, sizeof(bool) * V);
    int** sccs = NULL;
    int* sizes = NULL;
    int count = 0;

    while (!isEmpty(st))
    {
        int u = (int)(intptr_t)pop(st);
        if (!visited[u])
        {
            int* comp = NULL;
            int comp_size = 0;
            kosaraju_dfs2_vis(trans, u, visited, &comp, &comp_size);

            count++;
            int** new_sccs = realloc(sccs, sizeof(int*) * count);
            if (new_sccs == NULL)
            {
                free(comp);
                free_scc_result(sccs, sizes, count - 1);
                free_graph(trans);
                destroyStack(st, NULL);
                free(visited);
                return;
            }
            sccs = new_sccs;

            int* new_sizes = realloc(sizes, sizeof(int) * count);
            if (new_sizes == NULL)
            {
                free(comp);
                free_scc_result(sccs, sizes, count - 1);
                free_graph(trans);
                destroyStack(st, NULL);
                free(visited);
                return;
            }
            sizes = new_sizes;
            sccs[count - 1] = comp;
            sizes[count - 1] = comp_size;
        }
    }

    clear_screen();
    printf("\n=== Kosaraju's SCC Algorithm Final Result ===\n\n");
    printf("Total components found: %d\n", count);
    for (int i = 0; i < count; i++)
    {
        printf("Component %d: ", i + 1);
        for (int j = 0; j < sizes[i]; j++)
        {
            printf("%d ", sccs[i][j]);
        }
        printf("\n");
    }
    printf("\nPress Enter to continue...");
    fflush(stdout);
    getchar();

    free_graph(trans);
    destroyStack(st, NULL);
    free(visited);
    free_scc_result(sccs, sizes, count);
}
