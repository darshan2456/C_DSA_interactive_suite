#include "graph_traversals.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int greedy_best_first_search_solve(weightedGraph* graph, int start, int dest, int h[], int parent[],
                                   int traversal_order[], int* traversal_len)
{
    if (graph == NULL || start < 0 || start >= graph->V || dest < 0 || dest >= graph->V ||
        h == NULL || parent == NULL || traversal_order == NULL || traversal_len == NULL)
        return -1;

    int size = graph->V;
    int* visited = calloc(size, sizeof(int));
    int found = 0;
    PQ_graph pq = {0};

    if (!visited)
        goto cleanup;

    for (int i = 0; i < size; i++)
    {
        parent[i] = -1;
    }

    // Reuse the shared graph priority queue: a min-heap keyed on the node's
    // "distance" field, which here carries the heuristic h. Duplicate entries
    // are handled lazily via the visited[] check on pop.
    init_pq_graph(&pq, 10);

    if (!pq.heap)
    {
        found = -1;
        goto cleanup;
    }

    if (!insert_pq_graph(&pq, start, h[start]))
    {
        printf("Malloc failed\n");
        found = -1;
        goto cleanup;
    }

    *traversal_len = 0;

    PQ_graph_node popped;
    while (extractTop_pq_graph(&pq, &popped))
    {
        int u = popped.vertex;
        if (visited[u])
        {
            continue;
        }

        traversal_order[(*traversal_len)++] = u;

        // Display expansion details for learning/trace
        printf("[Expansion] Popped Node %d | h = %d\n", u, h[u]);

        if (u == dest)
        {
            found = 1;
            break;
        }

        visited[u] = 1;

        Edge* current = graph->array[u];
        while (current != NULL)
        {
            int v = current->destination;
            if (!visited[v] && parent[v] == -1 && v != start)
            {
                parent[v] = u;
                if (!insert_pq_graph(&pq, v, h[v]))
                {
                    printf("Malloc failed\n");
                    found = -1;
                    goto cleanup;
                }
            }
            current = current->next;
        }
    }

cleanup:
    PQ_Destroy(&pq);
    free(visited);
    return found;
}

// note: the time measured by clock() covers the search computation only (the
// greedy_best_first_search_solve call); it excludes path reconstruction and
// printing. it is for demonstration only and not a measure of efficiency.
void greedy_best_first_search(weightedGraph* graph, int start, int dest, int h[])
{
    int size = graph->V;
    int* parent = malloc(size * sizeof(int));
    int* traversal_order = malloc(size * sizeof(int));
    int traversal_len = 0;

    if (!parent || !traversal_order)
    {
        printf("Memory allocation failed in Greedy Best-First Search\n");
        free(parent);
        free(traversal_order);
        return;
    }

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();
    int found = greedy_best_first_search_solve(graph, start, dest, h, parent, traversal_order,
                                               &traversal_len);

    if (found == -1)
    {
        free(parent);
        free(traversal_order);
        return;
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    printf("\ntotal CPU time taken for greedy best-first search:- %f seconds\n", total_t);

    printf("Traversal Order: ");
    for (int i = 0; i < traversal_len; i++)
    {
        printf("%d", traversal_order[i]);
        if (i < traversal_len - 1)
        {
            printf(" -> ");
        }
    }
    printf("\n");

    if (!found)
    {
        printf("No path exists from %d to %d\n", start, dest);
    }
    else
    {
        int* path = malloc(size * sizeof(int));
        if (!path)
        {
            printf("Memory allocation failed in Greedy Best-First Search\n");
            free(parent);
            free(traversal_order);
            return;
        }
        int pathLen = 0;
        int curr = dest;
        while (curr != -1)
        {
            path[pathLen++] = curr;
            curr = parent[curr];
        }

        printf("Discovered Path: ");
        for (int i = pathLen - 1; i >= 0; i--)
        {
            printf("%d", path[i]);
            if (i > 0)
            {
                printf(" -> ");
            }
        }
        printf("\n");
        free(path);
    }

    free(parent);
    free(traversal_order);
}
