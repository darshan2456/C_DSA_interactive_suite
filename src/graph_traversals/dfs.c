#include "graph_traversals.h"
#include "safe_input.h"
#include "stack.h"
#include "step_debugger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void dfs(Graph* graph, int start);

// note: the time measured by clock() includes the traversal computation and the printing of each
// visited node. the CPU time is for demonstration only and must not be treated as a measure of the
// algorithm's efficiency.
void dfs(Graph* graph, int start)
{
    if (graph == NULL)
    {
        printf("\nError: NULL graph passed to DFS");
        return;
    }
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

    char msg[128];
    visited[start] = 1;
    snprintf(msg, sizeof(msg), "DFS: Visited start node %d", start);
    algorithm_step_hook(msg);
    push(nodes, start);

    while (1)
    { // main loop which performs dfs
        int curr = pop(nodes);
        if (curr == -1)
        {
            break;
        }
        snprintf(msg, sizeof(msg), "DFS: Popped node %d from stack", curr);
        algorithm_step_hook(msg);
        printf("%d->", curr);

        Node* temp = graph->array[curr];

        while (temp)
        {
            int v = temp->data;
            if (!visited[v])
            {
                visited[v] = 1;
                snprintf(msg, sizeof(msg), "DFS: Discovered node %d, pushing to stack", v);
                algorithm_step_hook(msg);
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