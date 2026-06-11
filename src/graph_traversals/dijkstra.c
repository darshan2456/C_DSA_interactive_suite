#include "graph_io.h"
#include "graph_traversals.h"
#include "history_logger.h"
#include "safe_input.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Min-heap ordering for PQ_graph: a node has higher priority when its
// "distance" (used as a generic priority: g+h for A*, h for Greedy, path
// cost for Dijkstra) is smaller. Equal priorities are broken by the lower
// vertex id so expansion order is deterministic and platform-independent.
static int pq_graph_higher_priority(PQ_graph_node a, PQ_graph_node b)
{
    if (a.distance != b.distance)
        return a.distance < b.distance;
    return a.vertex < b.vertex;
}

void init_pq_graph(PQ_graph* pq, int initial_capacity)
{
    if (pq == NULL)
        return;
    pq->size = 0;
    pq->capacity = initial_capacity > 0 ? initial_capacity : 10;
    pq->heap = malloc(pq->capacity * sizeof(PQ_graph_node));
}

void free_pq_graph(PQ_graph* pq)
{
    if (pq == NULL)
        return;
    free(pq->heap);
    pq->heap = NULL;
    pq->size = 0;
    pq->capacity = 0;
}

int insert_pq_graph(PQ_graph* pq, int vertex, int distance)
{
    if (pq == NULL || pq->heap == NULL)
        return 0;

    if (pq->size == pq->capacity)
    {
        int new_capacity = pq->capacity * 2;
        PQ_graph_node* new_heap = realloc(pq->heap, new_capacity * sizeof(PQ_graph_node));
        if (new_heap == NULL)
            return 0;
        pq->heap = new_heap;
        pq->capacity = new_capacity;
    }

    int i = pq->size;
    pq->heap[i].distance = distance;
    pq->heap[i].vertex = vertex;
    pq->size++;

    while (i > 0)
    {
        int parent = (i - 1) / 2;
        if (!pq_graph_higher_priority(pq->heap[i], pq->heap[parent]))
            break;

        PQ_graph_node temp = pq->heap[i];
        pq->heap[i] = pq->heap[parent];
        pq->heap[parent] = temp;

        i = parent;
    }

    return 1;
}

bool extractTop_pq_graph(PQ_graph* pq, PQ_graph_node* result)
{
    if (pq == NULL || result == NULL || pq->size == 0 || pq->heap == NULL)
        return false;

    int topIndex = 0;
    PQ_graph_node topElement = pq->heap[topIndex];
    int lastElementIndex = pq->size - 1;

    pq->heap[topIndex] = pq->heap[lastElementIndex];
    pq->size--;

    int i = 0;

    while (1)
    {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int target = i;

        if (left < pq->size && pq_graph_higher_priority(pq->heap[left], pq->heap[target]))
            target = left;
        if (right < pq->size && pq_graph_higher_priority(pq->heap[right], pq->heap[target]))
            target = right;

        if (target == i)
            break;

        PQ_graph_node temp = pq->heap[i];
        pq->heap[i] = pq->heap[target];
        pq->heap[target] = temp;

        i = target;
    }

    *result = topElement;

    return true;
}

// note: the time measured by clock() covers the shortest-path computation only
// (it stops before the distance table is printed). it is for demonstration only
// and must not be treated as a measure of the algorithm's efficiency.
void dijkstra(weightedGraph* graph, int start)
{
    int size = graph->V;
    int dist[size];

    for (int i = 0; i < size; i++)
        dist[i] = INT_MAX;

    dist[start] = 0;

    PQ_graph pq;
    init_pq_graph(&pq, 10);

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();

    insert_pq_graph(&pq, start, 0);

    PQ_graph_node currentNode;

    while (extractTop_pq_graph(&pq, &currentNode))
    {
        int u = currentNode.vertex;

        if (currentNode.distance > dist[u])
            continue;

        Edge* current = graph->array[u];

        while (current != NULL)
        {
            int v = current->destination;
            int currentWeight = current->weight;
            if (dist[u] != INT_MAX && dist[u] + currentWeight < dist[v])
            {
                dist[v] = dist[u] + currentWeight;
                insert_pq_graph(&pq, v, dist[v]);
            }

            current = current->next;
        }
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    free_pq_graph(&pq);

    printf("Start -> Vertex  \t  Distance\n");
    printf("---------------  \t  --------\n");

    for (int i = 0; i < size; i++)
    {
        if (dist[i] == INT_MAX)
            printf("    %d -> %d  \t            INF   \n", start, i);
        else
            printf("    %d -> %d  \t             %d   \n", start, i, dist[i]);
    }

    printf("\ntotal CPU time taken for Dijkstra's algorithm:- %f seconds\n", total_t);
    add_to_history("Dijkstra", size, total_t);
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
    int input_method;
    weightedGraph* graph = NULL;

    while (1)
    {
        int method_status = safe_input_int(&input_method,
                                           "\nenter 1 to build the graph manually, 2 to load it "
                                           "from a CSV file, enter '-1' to exit : ",
                                           1, 2);

        if (method_status == INPUT_EXIT_SIGNAL)
        {
            printf("\nExiting Dijkstra demo.....\n");
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
                printf("\nExiting Dijkstra demo.....\n");
                return;
            }

            if (len == 0)
            {
                continue;
            }

            graph = load_weightedGraph_from_csv(path);

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
                &edges, "\nenter number of edges (between 1 and 100), enter '-1' to exit :", 0,
                100);

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

        printf(
            "\nEnter source, destination, weight pairs (Source, Destination must be b/w 0 and %d "
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
