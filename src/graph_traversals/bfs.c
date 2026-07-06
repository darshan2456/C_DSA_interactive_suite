#include "data_structures.h"
#include "graph_traversals.h"
#include "returnMallocVal.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// note: the time measured by clock() includes the traversal computation and the printing of each
// visited node. the CPU time is for demonstration only and must not be treated as a measure of the
// algorithm's efficiency.
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

    Queue nodes;

    if (!init_circ_queue(size + 1, &nodes))
    {
        printf("\nerror initializing queue. returning....");
        return;
    }

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();

    visited[start] = 1;
    enqueue(&nodes, returnMallocInt(start));

    while (1)
    {
        int* curr_ptr = dequeue(&nodes);

        if (curr_ptr == NULL)
            break;

        int curr = *curr_ptr;
        free(curr_ptr);

        printf("%d->", curr);

        Node* temp = graph->array[curr];

        while (temp)
        {
            int v = temp->data;

            if (!visited[v])
            {
                visited[v] = 1;
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

void bfs_demo(void)
{
    int edges;
    int graph_capacity;
    int starting_node;
    int input_method;
    Graph* graph = NULL;

    while (1)
    {
        int method_status = safe_input_int(&input_method,
                                           "\nenter 1 to build the graph manually, 2 to load it "
                                           "from a CSV file, enter '-1' to exit : ",
                                           1, 2);

        if (method_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting bfs demo.....\n");
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
                printf("\nExiting bfs demo.....\n");
                return;
            }

            if (len == 0)
            {
                continue;
            }

            graph = load_graph_from_csv(path);

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
                &edges, "\nenter number of edges (between 1 and 100), enter '-1' to exit :", 1,
                100);

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