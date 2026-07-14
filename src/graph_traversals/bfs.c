#include "graph_traversals.h"
#include "queue.h"
#include "returnMallocVal.h"
#include "safe_input.h"
#include "step_debugger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// note: the time measured by clock() includes the traversal computation and the printing of each
// visited node. the CPU time is for demonstration only and must not be treated as a measure of the
// algorithm's efficiency.
void bfs(Graph* graph, int start)
{
    if (graph == NULL)
    {
        printf("\nError: NULL graph passed to BFS");
        return;
    }
    int size = graph->V;

    int visited[size];

    memset(visited, 0, sizeof(visited));

    if (start < 0 || start >= size)
    {
        printf("\ninvalid node passed as starting node");
        return;
    }

    Queue nodes;

    if (!init_circ_queue(size + 1, &nodes))
    {
        printf("\nerror initializing queue. returning....");
        return;
    }

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();

    char msg[128];
    visited[start] = 1;
    snprintf(msg, sizeof(msg), "BFS: Visited start node %d", start);
    algorithm_step_hook(msg);
    enqueue(&nodes, returnMallocInt(start));

    while (1)
    {
        int* curr_ptr = dequeue(&nodes);

        if (curr_ptr == NULL)
            break;

        int curr = *curr_ptr;
        free(curr_ptr);
        snprintf(msg, sizeof(msg), "BFS: Dequeued node %d", curr);
        algorithm_step_hook(msg);

        printf("%d->", curr);

        Node* temp = graph->array[curr];

        while (temp)
        {
            int v = temp->data;

            if (!visited[v])
            {
                visited[v] = 1;
                snprintf(msg, sizeof(msg), "BFS: Discovered node %d, enqueueing", v);
                algorithm_step_hook(msg);
                enqueue(&nodes, returnMallocInt(v));
            }

            temp = temp->next;
        }
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    printf("end\n");
    printf("\ntotal CPU time taken for BFS traversal:- %f seconds\n", total_t);

    destroy_circ_queue(&nodes);
}

Graph* create_graph(int V)
{
    Graph* graph = malloc(sizeof(Graph));
    if (!graph)
    {
        return NULL;
    }

    graph->V = V;

    graph->array = malloc(V * sizeof(Node*));

    if (!graph->array)
    {
        free(graph);
        return NULL;
    }

    for (int i = 0; i < V; i++)
    {
        graph->array[i] = NULL;
    }

    return graph;
}

void add_edge_undirected(Graph* graph, int src, int dest)
{
    if (!graph)
        return;

    if (src < 0 || src >= graph->V || dest < 0 || dest >= graph->V)
    {
        printf("Invalid edge: %d -> %d\n", src, dest);
        return;
    }

    sll_insertAtEnd(&graph->array[src], dest);
    sll_insertAtEnd(&graph->array[dest], src);
}

void free_graph(Graph* graph)
{
    if (!graph)
    {
        return;
    }
    for (int i = 0; i < graph->V; i++)
    {
        Node* temp = graph->array[i];

        while (temp)
        {
            Node* next = temp->next;
            free(temp);
            temp = next;
        }
    }

    free(graph->array);
    free(graph);
}

void add_edge_directed_unweighted(Graph* graph, int src, int dest)
{
    if (!graph)
        return;

    if (src < 0 || src >= graph->V || dest < 0 || dest >= graph->V)
    {
        printf("Invalid edge: %d -> %d\n", src, dest);
        return;
    }

    sll_insertAtEnd(&graph->array[src], dest);
}