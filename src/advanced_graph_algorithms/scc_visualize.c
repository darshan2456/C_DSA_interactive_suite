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
            if (temp->data == active_node)
            {
                printf("\033[1;33m%d\033[0m -> ", temp->data); // Yellow for reference
            }
            else
            {
                printf("%d -> ", temp->data);
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
                           int* time, int*** sccs, int** sizes, int* count)
{
    disc[u] = low[u] = ++(*time);
    push(st, u);
    on_stack[u] = true;

    print_graph_state(graph, u, disc, low, on_stack, st, "Tarjan DFS");

    Node* temp = graph->array[u];
    while (temp != NULL)
    {
        int v = temp->data;
        if (disc[v] == -1)
        {
            tarjan_dfs_vis(graph, v, disc, low, on_stack, st, time, sccs, sizes, count);
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
            return;
        }
        *sccs = new_sccs;

        int* new_sizes = realloc(*sizes, sizeof(int) * (*count));
        if (new_sizes == NULL)
        {
            free(comp);
            return;
        }
        *sizes = new_sizes;
        (*sccs)[*count - 1] = comp;
        (*sizes)[*count - 1] = comp_size;
    }
}

static void visualize_tarjan(Graph* graph)
{
    int V = graph->V;
    int* disc = malloc(sizeof(int) * V);
    int* low = malloc(sizeof(int) * V);
    bool* on_stack = malloc(sizeof(bool) * V);
    for (int i = 0; i < V; i++)
    {
        disc[i] = -1;
        low[i] = -1;
        on_stack[i] = false;
    }

    stack* st = createStack();
    int time = 0;
    int** sccs = NULL;
    int* sizes = NULL;
    int count = 0;

    for (int i = 0; i < V; i++)
    {
        if (disc[i] == -1)
        {
            tarjan_dfs_vis(graph, i, disc, low, on_stack, st, &time, &sccs, &sizes, &count);
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

    destroyStack(st);
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
        int v = temp->data;
        if (!visited[v])
        {
            kosaraju_dfs1_vis(graph, v, visited, st, phase);
        }
        temp = temp->next;
    }
    push(st, u);
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
        int v = temp->data;
        if (!visited[v])
        {
            kosaraju_dfs2_vis(graph, v, visited, comp, comp_size);
        }
        temp = temp->next;
    }
}

static void visualize_kosaraju(Graph* graph)
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
        int u = pop(st);
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
                destroyStack(st);
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
                destroyStack(st);
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
    destroyStack(st);
    free(visited);
    free_scc_result(sccs, sizes, count);
}

void scc_demo(void)
{
    int edges;
    int graph_capacity;
    int input_method;
    Graph* graph = NULL;

    while (1)
    {
        int method_status = safe_input_int(&input_method,
                                           "\nenter 1 to build the graph manually, 2 to load it "
                                           "from a CSV file, enter '-1' to exit : ",
                                           1, 2);

        if (method_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting SCC visualization demo.....\n");
            return;
        }

        if (method_status == 0)
        {
            continue;
        }

        break;
    }

    if (input_method == 2)
    {
        while (1)
        {
            char path[256];
            printf("\nenter the path to the CSV file, enter '-1' to exit : ");
            fflush(stdout);

            if (!fgets(path, sizeof(path), stdin))
            {
                printf("\ninput ended unexpectedly\n");
                clearerr(stdin);
                return;
            }

            size_t len = strlen(path);
            while (len > 0 && (path[len - 1] == '\n' || path[len - 1] == '\r'))
                path[--len] = '\0';

            if (strcmp(path, "-1") == 0)
            {
                printf("\nExiting SCC visualization demo.....\n");
                return;
            }

            if (len == 0)
            {
                continue;
            }

            graph = load_graph_from_csv(path);

            if (!graph)
            {
                continue;
            }

            break;
        }
        graph_capacity = graph->V;
    }
    else if (input_method == 1)
    {
        while (1)
        {
            int graph_capacity_status =
                safe_input_int(&graph_capacity,
                               "\nenter the number of vertices in the graph, "
                               "(between 1 and 10), enter '-1' to exit : ",
                               1, 10);

            if (graph_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting SCC visualization demo.....\n");
                return;
            }

            if (graph_capacity_status == 0)
            {
                continue;
            }

            graph = create_graph(graph_capacity);
            if (!graph)
            {
                printf("\nmalloc allocation failed\n");
                return;
            }

            break;
        }

        while (1)
        {
            int edges_capacity_status = safe_input_int(
                &edges,
                "\nenter number of directed edges (between 1 and 100), enter '-1' to exit :", 1,
                100);

            if (edges_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting SCC visualization demo\n");
                free_graph(graph);
                return;
            }

            if (edges_capacity_status == 0)
            {
                continue;
            }

            break;
        }

        printf("\nenter directed edges (src dest) (vertices from 0 to %d, enter '-1' to exit):\n",
               graph_capacity - 1);

        for (int i = 0; i < edges; i++)
        {
            int src_status;
            int dest_status;
            int src;
            int dest;

        retry:
            src_status = safe_input_int(&src, "src: ", 0, graph_capacity - 1);

            if (src_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting SCC visualization demo\n");
                free_graph(graph);
                return;
            }
            if (src_status == 0)
            {
                goto retry;
            }

            dest_status = safe_input_int(&dest, "dest: ", 0, graph_capacity - 1);

            if (dest_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting SCC visualization demo\n");
                free_graph(graph);
                return;
            }
            if (dest_status == 0)
            {
                goto retry;
            }

            add_edge_directed_unweighted(graph, src, dest);
        }
    }

    int choice;
    while (1)
    {
        int choice_status = safe_input_int(&choice,
                                           "\nChoose SCC Algorithm to Visualize:\n"
                                           "1. Tarjan's Algorithm\n"
                                           "2. Kosaraju's Algorithm\n"
                                           "Enter choice (-1 to exit): ",
                                           1, 2);
        if (choice_status == INPUT_EXIT_SIGNAL)
        {
            break;
        }
        if (choice_status == 0)
        {
            continue;
        }
        if (choice == 1)
        {
            visualize_tarjan(graph);
        }
        else if (choice == 2)
        {
            visualize_kosaraju(graph);
        }
        break;
    }

    free_graph(graph);
}
