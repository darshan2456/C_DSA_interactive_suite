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

void astar_demo(void)
{
    int edges;
    int graph_capacity;
    int starting_node;
    int destination_node;
    int input_method;
    weightedGraph* graph = NULL;
    int* h = NULL;

    while (1)
    {
        int method_status = safe_input_int(&input_method,
                                           "\nenter 1 to build the graph manually, 2 to load it "
                                           "from a CSV file, enter '-1' to exit : ",
                                           1, 2);

        if (method_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting A* demo.....\n");
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
                printf("\nExiting A* demo.....\n");
                return;
            }

            if (len == 0)
            {
                continue;
            }

            // Loads the graph and its per-vertex heuristic together, so the
            // manual heuristic-entry block below is skipped on this path.
            graph = load_weightedGraph_with_heuristic_from_csv(path, &h);

            if (!graph)
            {
                // error already reported by the loader, let the user retry
                continue;
            }

            break;
        }

        graph_capacity = graph->V;
    }
    else
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
                printf("\nExiting A* demo.....\n");
                return;
            }

            if (graph_capacity_status == 0)
            {
                continue;
            }

            graph = create_weightedGraph(graph_capacity);

            if (!graph)
            {
                printf("\nmemory allocation failed\n");
                return;
            }

            break;
        }

        while (1)
        {
            int edges_capacity_status = safe_input_int(
                &edges, "\nenter number of edges (between 1 and 100), enter '-1' to exit :", 0,
                100);

            if (edges_capacity_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo\n");
                free_weightedGraph(graph);
                return;
            }

            if (edges_capacity_status == 0)
            {
                continue;
            }

            break;
        }

        printf(
            "\nEnter source, destination, weight pairs (Source, Destination must be b/w 0 and %d "
            "(both inclusive)):\n",
            graph_capacity - 1);

        for (int i = 0; i < edges; i++)
        {
            int src_status;
            int dest_status;
            int wt_status;
            int src;
            int dest;
            int wt;

        retry:
            src_status = safe_input_int(&src, "src: ", 0, graph_capacity - 1);

            if (src_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo\n");
                free_weightedGraph(graph);
                return;
            }
            if (src_status == 0)
            {
                goto retry;
            }

            dest_status = safe_input_int(&dest, "dest: ", 0, graph_capacity - 1);

            if (dest_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo\n");
                free_weightedGraph(graph);
                return;
            }
            if (dest_status == 0)
            {
                goto retry;
            }

            wt_status = safe_input_int(&wt, "weight: ", 0, INT_MAX);

            if (wt_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo\n");
                free_weightedGraph(graph);
                return;
            }
            if (wt_status == 0)
            {
                goto retry;
            }

            add_edge_directed(graph, src, dest, wt);
        }
    }

    while (1)
    {
        if (!h)
        {
            h = malloc(graph_capacity * sizeof(int));
            if (!h)
            {
                printf("\nmemory allocation failed for heuristics\n");
                free_weightedGraph(graph);
                return;
            }

            printf("\nEnter heuristic values for each vertex:\n");
            for (int i = 0; i < graph_capacity; i++)
            {
                int h_val;
                int h_status;
                char prompt[50];
                snprintf(prompt, sizeof(prompt), "h(%d): ", i);
            retry_h:
                h_status = safe_input_int(&h_val, prompt, 0, INT_MAX);
                if (h_status == INPUT_EXIT_SIGNAL)
                {
                    printf("\nExiting A* demo\n");
                    free(h);
                    free_weightedGraph(graph);
                    return;
                }
                if (h_status == 0)
                {
                    goto retry_h;
                }
                h[i] = h_val;
            }
        }

        while (1)
        {
            int start_status =
                safe_input_int(&starting_node, "\nenter starting node: ", 0, graph_capacity - 1);

            if (start_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo.....\n");
                free(h);
                free_weightedGraph(graph);
                return;
            }

            if (start_status == 0)
            {
                continue;
            }

            break;
        }

        while (1)
        {
            int dest_status = safe_input_int(&destination_node, "\nenter destination node: ", 0,
                                             graph_capacity - 1);

            if (dest_status == INPUT_EXIT_SIGNAL)
            {
                printf("\nExiting A* demo.....\n");
                free(h);
                free_weightedGraph(graph);
                return;
            }

            if (dest_status == 0)
            {
                continue;
            }

            break;
        }

        printf("\n");
        astar(graph, starting_node, destination_node, h);

        int choice;
    retry_choice:
        printf("\nOptions:\n1. Re-run A* with NEW heuristics\n2. Re-run A* with SAME heuristics "
               "(new start/destination)\n0. Exit A* demo\n");
        int choice_status =
            safe_input_int(&choice, "\nenter choice (\'-1\' to exit, or \'help\') : ", 0, 2);
        if (choice_status == INPUT_EXIT_SIGNAL || choice == 0)
        {
            printf("\nExiting A* demo.....\n");
            free(h);
            free_weightedGraph(graph);
            return;
        }
        if (choice_status == 0)
        {
            goto retry_choice;
        }

        if (choice == 1)
        {
            free(h);
            h = NULL;
        }
    }
}
