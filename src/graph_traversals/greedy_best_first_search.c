#include "graph_traversals.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int h;
    int node;
} HeapNode;

typedef struct
{
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

static MinHeap* create_min_heap(int capacity)
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

static void free_min_heap(MinHeap* heap)
{
    if (heap)
    {
        free(heap->data);
        free(heap);
    }
}

static void swap_heap_nodes(HeapNode* a, HeapNode* b)
{
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

static int compare_nodes(HeapNode a, HeapNode b)
{
    if (a.h < b.h)
    {
        return -1;
    }
    if (a.h > b.h)
    {
        return 1;
    }
    // Tie-break: prefer lower node ID
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

static void heapify_up(MinHeap* heap, int idx)
{
    while (idx > 0)
    {
        int parent = (idx - 1) / 2;
        if (compare_nodes(heap->data[idx], heap->data[parent]) < 0)
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

static void heapify_down(MinHeap* heap, int idx)
{
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && compare_nodes(heap->data[left], heap->data[smallest]) < 0)
    {
        smallest = left;
    }
    if (right < heap->size && compare_nodes(heap->data[right], heap->data[smallest]) < 0)
    {
        smallest = right;
    }

    if (smallest != idx)
    {
        swap_heap_nodes(&heap->data[idx], &heap->data[smallest]);
        heapify_down(heap, smallest);
    }
}

static int heap_push(MinHeap* heap, int h_val, int node)
{
    if (heap->size >= heap->capacity)
    {
        return 0;
    }
    heap->data[heap->size].h = h_val;
    heap->data[heap->size].node = node;
    heapify_up(heap, heap->size);
    heap->size++;
    return 1;
}

static int heap_pop(MinHeap* heap, HeapNode* popped)
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
        heapify_down(heap, 0);
    }
    return 1;
}

int greedy_best_first_search_solve(weightedGraph* graph, int start, int dest, int h[], int parent[], int traversal_order[], int* traversal_len)
{
    int size = graph->V;
    int* visited = calloc(size, sizeof(int));
    if (!visited)
    {
        return 0;
    }

    MinHeap* heap = create_min_heap(size * size + 5);
    if (!heap)
    {
        free(visited);
        return 0;
    }

    for (int i = 0; i < size; i++)
    {
        parent[i] = -1;
    }

    heap_push(heap, h[start], start);

    int found = 0;
    *traversal_len = 0;

    while (heap->size > 0)
    {
        HeapNode popped;
        if (!heap_pop(heap, &popped))
        {
            break;
        }

        int u = popped.node;
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
                heap_push(heap, h[v], v);
            }
            current = current->next;
        }
    }

    free_min_heap(heap);
    free(visited);
    return found;
}

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

    int found = greedy_best_first_search_solve(graph, start, dest, h, parent, traversal_order, &traversal_len);

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

void greedy_best_first_search_demo(void)
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
            printf("\nExiting Greedy Best-First Search demo.....\n");
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
            printf("\nExiting Greedy Best-First Search demo\n");
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
            printf("\nExiting Greedy Best-First Search demo\n");
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
            printf("\nExiting Greedy Best-First Search demo\n");
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
            printf("\nExiting Greedy Best-First Search demo\n");
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
                    printf("\nExiting Greedy Best-First Search demo\n");
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
                printf("\nExiting Greedy Best-First Search demo.....\n");
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
                printf("\nExiting Greedy Best-First Search demo.....\n");
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
        greedy_best_first_search(graph, starting_node, destination_node, h);

        int choice;
    retry_choice:
        printf("\nOptions:\n1. Re-run Greedy BFS with NEW heuristics\n2. Re-run Greedy BFS with SAME heuristics (new start/destination)\n0. Exit Greedy BFS demo\n");
        int choice_status = safe_input_int(&choice, "Enter choice: ", 0, 2);
        if (choice_status == INPUT_EXIT_SIGNAL || choice == 0)
        {
            printf("\nExiting Greedy Best-First Search demo.....\n");
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
