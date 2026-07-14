#include "graph_traversals.h"
#include "queue.h"
#include "returnMallocVal.h"
#include "safe_input.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void topological_sort_kahn(Graph* graph)
{
    if (graph == NULL)
    {
        printf("\nError: NULL graph passed to Topological Sort");
        return;
    }
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
        printf("Cycle detected! Only %d of %d vertices were processed.\n", processed, size);
        printf("The graph is NOT a DAG -- topological sort is not possible.\n");
    }
    else
    {
        printf("All %d vertices processed. The graph is a valid DAG.\n", size);
    }

    destroy_circ_queue(&q);
}
