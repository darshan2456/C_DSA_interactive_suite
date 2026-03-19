#include "graph_traversals.h"
#include "safe_input.h"
#include "stack.h"
#include <stdio.h>
#include <string.h>

void dfs(Graph* graph, int start);

void dfs_demo(void)
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
            printf("\nExiting dfs demo.....\n");
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
            printf("\nExiting dfs demo\n");
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
            printf("\nExiting dfs demo\n");
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
            printf("\nExiting dfs demo\n");
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
            printf("\nExiting dfs demo.....\n");
            free_graph(graph);
            return;
        }
        if (start_status == 0)
            continue;

        if (starting_node < 0 || starting_node >= graph->V)
        {
            printf("Invalid start node\n");
            return;
        }
        break;
    }

    dfs(graph, starting_node);
    free_graph(graph);
}
void dfs(Graph* graph, int start)
{
    int size = graph->V;

    int visited[size];

    memset(visited, 0, sizeof(visited)); // initializing the array with all zeroes

    if (start < 0 || start >= size)
    {
        printf("\ninvalid node passed as starting node");
        return;
    }

    stack* nodes = createStack();

    if (nodes == NULL)
    {
        printf("stack could not be initialized due to a malloc failure");
        return;
    }

    visited[start] = 1;
    push(nodes, start);

    while (1)
    { // main loop which performs dfs
        int curr = pop(nodes);
        if (curr == -1)
        {
            break;
        }
        printf("%d->", curr);

        Node* temp = graph->array[curr];

        while (temp)
        {
            int v = temp->data;
            if (!visited[v])
            {
                visited[v] = 1;
                push(nodes, v);
            }
            temp = temp->next;
        }
    }
    printf("end\n");
    destroyStack(nodes);
    return;
}