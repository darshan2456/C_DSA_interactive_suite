#include "data_structures.h"
#include "safe_input.h"
#include "returnMallocVal.h"
#include <stdio.h>
#include "graph_traversals.h"
#include <stdlib.h>
#include <string.h>

void topological_sort_kahn(Graph* graph)
{
    int size = graph->V;

    int in_degree[size];
    memset(in_degree, 0, sizeof(in_degree));

    /* Step 1: Compute in-degrees */
    for (int u = 0; u < size; u++)
    {
        Node* temp = graph->array[u];

        while (temp)
        {
            in_degree[temp->data]++;
            temp = temp->next;
        }
    }

    /* Step 2: Enqueue all vertices with in-degree 0 */
    Queue q;

    if (!init_circ_queue(size + 1, &q))
    {
        printf("\nerror initializing queue. returning....\n");
        return;
    }

    for (int i = 0; i < size; i++)
    {
        if (in_degree[i] == 0)
            enqueue(&q, returnMallocInt(i));
    }

    /* Step 3: Process the queue (Kahn's BFS) */
    int processed = 0;

    printf("\nTopological Order: ");

    while (1)
    {
        int* u_ptr = dequeue(&q);

        if (u_ptr == NULL)
            break;

        int u = *u_ptr;
        free(u_ptr);

        printf("%d ", u);
        processed++;

        Node* temp = graph->array[u];

        while (temp)
        {
            int v = temp->data;

            in_degree[v]--;

            if (in_degree[v] == 0)
                enqueue(&q, returnMallocInt(v));

            temp = temp->next;
        }
    }

    printf("\n");

    /* Step 4: Cycle detection */
    if (processed != size)
    {
        printf("Cycle detected! Only %d of %d vertices were processed.\n",
               processed, size);
        printf("The graph is NOT a DAG -- topological sort is not possible.\n");
    }
    else
    {
        printf("All %d vertices processed. The graph is a valid DAG.\n", size);
    }

    destroy_circ_queue(&q);
}

void topological_sort_demo(void)
{
    int edges;
    int graph_capacity;
    Graph* graph = NULL;

    while (1)
    {
        int graph_capacity_status = safe_input_int(&graph_capacity,
                                                   "\nenter the number of vertices in the graph, "
                                                   "(between 1 and 10), enter '-1' to exit : ",
                                                   1, 10);

        if (graph_capacity_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting topological sort demo.....\n");
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
            &edges, "\nenter number of directed edges (between 1 and 100), enter '-1' to exit :", 1,
            100);

        if (edges_capacity_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting topological sort demo\n");
            free_graph(graph);
            return;
        }

        if (edges_capacity_status == 0)
        {
            continue;
        }

        break;
    }

    printf("\nenter directed edges (src dest) (from 0 to vertices-1, enter '-1' to exit):\n");

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
            printf("\nExiting topological sort demo\n");
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
            printf("\nExiting topological sort demo\n");
            free_graph(graph);
            return;
        }
        if (dest_status == 0)
        {
            goto retry;
        }

        add_edge_directed_unweighted(graph, src, dest);
    }

    topological_sort_kahn(graph);
    free_graph(graph);
}