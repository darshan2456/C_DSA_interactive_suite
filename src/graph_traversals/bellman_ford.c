#include "graph_traversals.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>

void bellman_ford(weightedGraph* graph, int start)
{
    int size = graph->V;

    int dist[size];

    for(int i = 0 ; i < size ; i++)
        dist[i] = INT_MAX;

    dist[start] = 0;

    for(int count = 0 ; count < size-1 ; count++)
    {
        for(int u = 0 ; u < size ; u++)
        {
            Edge* current = graph->array[u];

            while(current != NULL)
            {
                int v = current->destination;
                int weight = current->weight;

                if(dist[u] != INT_MAX && dist[u] + weight < dist[v])
                    dist[v] = dist[u] + weight;

                current = current->next;
            }
        }
    }

    // Negative edge cycle detector
    for(int u = 0 ; u < size ; u++)
    {
        Edge* current = graph->array[u];
        while(current != NULL)
        {
            int v = current->destination;
            int weight = current->weight;
            if(dist[u] != INT_MAX && dist[u] + weight < dist[v])
            {
                printf("Negative edge cycle detected. Graph is not suitable for bellman ford.\n");
                return;
            }

            current = current->next;
        }
    }

    // Print vertex, distance stuff
    printf("Start -> Vertex  \t  Distance\n");
    printf("---------------  \t  --------\n");

    for(int i = 0 ; i < size ; i++)
    {
        if(dist[i] == INT_MAX)
            printf("    %d -> %d  \t            INF   \n",start,i);
        else
            printf("    %d -> %d  \t             %d   \n",start,i,dist[i]);
    }
}

void bellman_ford_demo(void)
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
            printf("\nExiting Bellman Ford demo.....\n");
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
            printf("\nExiting Bellman Ford demo\n");
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
            printf("\nExiting Bellman Ford demo\n");
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

        wt_status = safe_input_int(&wt, "weight: ", INT_MIN, INT_MAX);

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
            printf("\nExiting Bellman Ford demo.....\n");
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
    bellman_ford(graph, starting_node);
    free_weightedGraph(graph);
}