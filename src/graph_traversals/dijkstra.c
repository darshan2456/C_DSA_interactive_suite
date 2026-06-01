#include "graph_traversals.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int minDistance(int visited[], int dist[], int size)
{
    int minDist = INT_MAX;
    int u = -1;
    for (int i = 0; i < size; i++)
    {
        if (!visited[i] && dist[i] < minDist)
        {
            minDist = dist[i];
            u = i;
        }
    }

    return u;
}

void dijkstra(weightedGraph* graph, int start)
{
    int size = graph->V;

    int visited[size];
    int dist[size];

    for (int i = 0; i < size; i++)
    {
        visited[i] = 0;
        dist[i] = INT_MAX;
    }

    dist[start] = 0;

    for (int count = 0; count < size - 1; count++)
    {
        int u = minDistance(visited, dist, size);
        visited[u] = 1;

        if (u == -1)
            break;

        Edge* current = graph->array[u];

        while (current != NULL)
        {
            int v = current->destination;
            int currentWeight = current->weight;
            if (!visited[v] && dist[u] != INT_MAX && dist[u] + currentWeight < dist[v])
                dist[v] = dist[u] + currentWeight;

            current = current->next;
        }
    }

    printf("Start -> Vertex  \t  Distance\n");
    printf("---------------  \t  --------\n");

    for (int i = 0; i < size; i++)
    {
        if (dist[i] == INT_MAX)
            printf("    %d -> %d  \t            INF   \n", start, i);
        else
            printf("    %d -> %d  \t             %d   \n", start, i, dist[i]);
    }
}

weightedGraph* create_weightedGraph(int V)
{
    weightedGraph* graph = malloc(sizeof(weightedGraph));

    if (!graph)
        return NULL;

    graph->V = V;

    graph->array = malloc(V * sizeof(Edge*));

    if (!graph->array)
    {
        free(graph);
        return NULL;
    }

    for (int i = 0; i < V; i++)
        graph->array[i] = NULL;

    return graph;
}

int edge_insertAtEnd(Edge** head, int dest, int weight)
{
    Edge* edge = malloc(sizeof(Edge));

    if (!edge)
        return -1;

    edge->destination = dest;
    edge->weight = weight;
    edge->next = NULL;

    if (*head == NULL)
    {
        *head = edge;
        return 1;
    }

    Edge* temp = *head;

    while (temp->next != NULL)
        temp = temp->next;

    temp->next = edge;
    return 1;
}

void add_edge_directed(weightedGraph* graph, int src, int dest, int wt)
{
    if (!graph)
        return;

    if (src < 0 || src >= graph->V || dest < 0 || dest >= graph->V || wt < 0)
    {
        printf("Invalid edge: %d -> %d\n", src, dest);
    }

    edge_insertAtEnd(&graph->array[src], dest, wt);
}

void free_weightedGraph(weightedGraph* graph)
{
    if (!graph)
        return;

    for (int i = 0; i < graph->V; i++)
    {
        Edge* temp = graph->array[i];
        while (temp != NULL)
        {
            Edge* prev = temp;
            temp = temp->next;
            free(prev);
        }
    }

    free(graph->array);
    free(graph);
}

void dijkstra_demo(void)
{
    int edges;
    int graph_capacity;
    int starting_node;
    weightedGraph* graph = NULL;

    while (1)
    {
        int graph_capacity_status = safe_input_int(&graph_capacity,
                                                   "\nenter the number of vertices in the graph, "
                                                   "(between 1 and 10), enter '-1' to exit : ",
                                                   1, 10);

        if (graph_capacity_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Dijkstra demo.....\n");
            free_weightedGraph(graph);
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
            free_weightedGraph(graph);
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
            printf("\nExiting Dijkstra demo\n");
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
            printf("\nExiting Dijkstra demo\n");
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
            printf("\nExiting Dijsktra demo\n");
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
            printf("\nExiting Dijsktra demo\n");
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
        int start_status =
            safe_input_int(&starting_node, "\nenter starting node: ", 0, graph_capacity - 1);

        if (start_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Dijkstra demo.....\n");
            free_weightedGraph(graph);
            return;
        }

        if (start_status == 0)
            continue;

        if (starting_node < 0 || starting_node >= graph->V)
        {
            printf("Invalid start node\n");
            free_weightedGraph(graph);
            return;
        }
        break;
    }

    printf("\n");
    dijkstra(graph, starting_node);
    free_weightedGraph(graph);
}
