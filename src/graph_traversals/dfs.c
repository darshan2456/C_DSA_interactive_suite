#include "graph_traversals.h"
#include "safe_input.h"
#include <stdio.h>

#include <string.h>
#include <time.h>

void dfs(Graph* graph, int start);

void dfs_demo(void)
{
    int edges;
    int graph_capacity = 0;
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
            printf("\nExiting dfs demo.....\n");
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
                printf("\nExiting dfs demo.....\n");
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
    else if (input_method == 1)
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
                &edges, "\nenter number of edges (between 1 and 100), enter '-1' to exit :", 1,
                100);

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
            free_graph(graph);
            return;
        }
        break;
    }

    dfs(graph, starting_node);
    free_graph(graph);
}
// note: the time measured by clock() includes the traversal computation and the printing of each
// visited node. the CPU time is for demonstration only and must not be treated as a measure of the
// algorithm's efficiency.
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

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();

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

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    printf("end\n");
    printf("\ntotal CPU time taken for DFS traversal:- %f seconds\n", total_t);
    destroyStack(nodes);
    return;
}