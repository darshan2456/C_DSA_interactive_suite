#include "circular_queue.h"
#include "graph_traversals.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bfs(Graph* graph, int start)
{
    int size = graph->V;

    int visited[size];

    memset(visited, 0, sizeof(visited));

    if (start < 0 || start >= size)
    {
        printf("\ninvalid node passed as starting node");
        return;
    }

    circular_queue nodes;

    if (!init_circ_queue(size + 1, &nodes))
    {
        printf("\nerror initializing queue. returning....");
        return;
    }

    visited[start] = 1;
    enqueue(&nodes, start);

    while (1)
    {
        int curr = dequeue(&nodes);
        if (curr == -1)
            break;

        printf("%d->", curr);

        Node* temp = graph->array[curr];

        while (temp)
        {
            int v = temp->data;
            if (!visited[v])
            {
                visited[v] = 1;
                enqueue(&nodes, v);
            }
            temp = temp->next;
        }
    }

    printf("end\n");
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

void bfs_demo(void)
{
    int edges;
    int graph_capacity;
    int starting_node;
    Graph* graph = NULL;

    while (1)
    {
        int graph_capacity_status = safe_input_int(&graph_capacity,
                                                   "\nenter the number of vertices in the graph, "
                                                   "(between 1 and 10), enter '-1' to exit : ",
                                                   1, 10);

        if (graph_capacity_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting bfs demo.....\n");
            free_graph(graph);
            return;
        }

        if (graph_capacity_status == 0)
        {
            continue;
        }

        graph = create_graph(graph_capacity);

        if (!graph)
        {
            printf("\nmemory allocation failed\n");
            free_graph(graph);
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
            printf("\nExiting bfs demo\n");
            free_graph(graph);
            return;
        }

        if (edges_capacity_status == 0)
        {
            continue;
        }

        break;
    }

    printf("\nenter edges (src dest) (from 0 to vertices-1, enter '-1' to exit):\n");

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
            printf("\nExiting bfs demo\n");
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
            printf("\nExiting bfs demo\n");
            free_graph(graph);
            return;
        }
        if (dest_status == 0)
        {
            goto retry;
        }

        add_edge_undirected(graph, src, dest);
    }

    while (1)
    {
        int start_status =
            safe_input_int(&starting_node, "\nenter starting node: ", 0, graph_capacity - 1);

        if (start_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting bfs demo.....\n");
            free_graph(graph);
            return;
        }
        if (start_status == 0)
            continue;

        if (starting_node < 0 || starting_node >= graph->V)
        {
            printf("Invalid start node\n");
            free_graph(graph);
            return;
        }
        break;
    }

    bfs(graph, starting_node);
    free_graph(graph);
}