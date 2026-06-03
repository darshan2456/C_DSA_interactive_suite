#include "graph_traversals.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int f;
    int node;
} HeapNode;

typedef struct
{
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_min_heap(int capacity)
{
    MinHeap* heap = malloc(sizeof(MinHeap));
    if (!heap)
    {
        return NULL;
    }
    heap->data = malloc(capacity * sizeof(HeapNode));
    if (!heap->data)
    {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void free_min_heap(MinHeap* heap)
{
    if (heap)
    {
        free(heap->data);
        free(heap);
    }
}

void swap_heap_nodes(HeapNode* a, HeapNode* b)
{
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

int compare_nodes(HeapNode a, HeapNode b, int h[])
{
    if (a.f < b.f)
    {
        return -1;
    }
    if (a.f > b.f)
    {
        return 1;
    }
    // Tie-break: prefer lower h value (more goal-directed)
    if (h[a.node] < h[b.node])
    {
        return -1;
    }
    if (h[a.node] > h[b.node])
    {
        return 1;
    }
    // Further tie-break: prefer lower node ID
    if (a.node < b.node)
    {
        return -1;
    }
    if (a.node > b.node)
    {
        return 1;
    }
    return 0;
}

void heapify_up(MinHeap* heap, int idx, int h[])
{
    while (idx > 0)
    {
        int parent = (idx - 1) / 2;
        if (compare_nodes(heap->data[idx], heap->data[parent], h) < 0)
        {
            swap_heap_nodes(&heap->data[idx], &heap->data[parent]);
            idx = parent;
        }
        else
        {
            break;
        }
    }
}

void heapify_down(MinHeap* heap, int idx, int h[])
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && compare_nodes(heap->data[left], heap->data[smallest], h) < 0)
    {
        smallest = left;
    }
    if (right < heap->size && compare_nodes(heap->data[right], heap->data[smallest], h) < 0)
    {
        smallest = right;
    }

    if (smallest != idx)
    {
        swap_heap_nodes(&heap->data[idx], &heap->data[smallest]);
        heapify_down(heap, smallest, h);
    }
}

int heap_push(MinHeap* heap, int f, int node, int h[])
{
    if (heap->size >= heap->capacity)
    {
        return 0;
    }
    heap->data[heap->size].f = f;
    heap->data[heap->size].node = node;
    heapify_up(heap, heap->size, h);
    heap->size++;
    return 1;
}

int heap_pop(MinHeap* heap, HeapNode* popped, int h[])
{
    if (heap->size <= 0)
    {
        return 0;
    }
    *popped = heap->data[0];
    heap->size--;
    if (heap->size > 0)
    {
        heap->data[0] = heap->data[heap->size];
        heapify_down(heap, 0, h);
    }
    return 1;
}

int astar_solve(weightedGraph* graph, int start, int dest, int h[], int parent[])
{
    int size = graph->V;
    int* visited = calloc(size, sizeof(int));
    int* dist = malloc(size * sizeof(int));
    int* fScore = malloc(size * sizeof(int));

    if (!visited || !dist || !fScore)
    {
        free(visited);
        free(dist);
        free(fScore);
        return -1;
    }

    // Allocate heap to hold up to size * size entries (to support duplicates)
    MinHeap* heap = create_min_heap(size * size + 5);
    if (!heap)
    {
        free(visited);
        free(dist);
        free(fScore);
        return -1;
    }

    for (int i = 0; i < size; i++)
    {
        dist[i] = INT_MAX;
        fScore[i] = INT_MAX;
        if (parent)
        {
            parent[i] = -1;
        }
    }

    dist[start] = 0;
    fScore[start] = h[start];
    heap_push(heap, fScore[start], start, h);

    int result = INT_MAX;

    while (heap->size > 0)
    {
        HeapNode popped;
        if (!heap_pop(heap, &popped, h))
        {
            break;
        }

        int u = popped.node;

        if (visited[u])
        {
            continue;
        }

        // Display expansion details for learning/trace
        printf("[Expansion] Popped Node %d | g = %d, h = %d, f = %d\n", u, dist[u], h[u], popped.f);

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

                    heap_push(heap, fScore[v], v, h);
                }
            }
            current = current->next;
        }
    }

    free_min_heap(heap);
    free(visited);
    free(dist);
    free(fScore);
    return result;
}

void astar(weightedGraph* graph, int start, int dest, int h[])
{
    int size = graph->V;
    int* parent = malloc(size * sizeof(int));
    if (!parent)
    {
        printf("Memory allocation failed in A*\n");
        return;
    }

    int cost = astar_solve(graph, start, dest, h, parent);

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
    weightedGraph* graph = NULL;
    int* h = NULL;

    while (1)
    {
        int graph_capacity_status = safe_input_int(&graph_capacity,
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
            &edges, "\nenter number of edges (between 1 and 100), enter '-1' to exit :", 0, 100);

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

    printf("\nEnter source, destination, weight pairs (Source, Destination must be b/w 0 and %d "
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
                sprintf(prompt, "h(%d): ", i);
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
            int dest_status =
                safe_input_int(&destination_node, "\nenter destination node: ", 0, graph_capacity - 1);

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
        printf("\nOptions:\n1. Re-run A* with NEW heuristics\n2. Re-run A* with SAME heuristics (new start/destination)\n0. Exit A* demo\n");
        int choice_status = safe_input_int(&choice, "Enter choice: ", 0, 2);
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
