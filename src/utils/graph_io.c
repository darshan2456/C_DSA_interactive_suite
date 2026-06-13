#include "graph_io.h"
#include <stdio.h>
#include <stdlib.h>

// Vertex count is capped to match the manual-input path in dijkstra_demo so
// both input methods accept the same range of graphs.
#define CSV_MAX_VERTICES 10
#define CSV_MAX_EDGES 100

weightedGraph* load_weightedGraph_from_csv(const char* path)
{
    FILE* fp = fopen(path, "r");

    if (!fp)
    {
        printf("\nCould not open file '%s'. Please check the path and try again.\n", path);
        return NULL;
    }

    int V;
    int E;

    if (fscanf(fp, "%d %d", &V, &E) != 2)
    {
        printf("\nInvalid file: the first line must be '<vertices> <edges>'.\n");
        fclose(fp);
        return NULL;
    }

    if (V < 1 || V > CSV_MAX_VERTICES)
    {
        printf("\nInvalid vertex count %d (must be between 1 and %d).\n", V, CSV_MAX_VERTICES);
        fclose(fp);
        return NULL;
    }

    if (E < 0 || E > CSV_MAX_EDGES)
    {
        printf("\nInvalid edge count %d (must be between 0 and %d).\n", E, CSV_MAX_EDGES);
        fclose(fp);
        return NULL;
    }

    weightedGraph* graph = create_weightedGraph(V);

    if (!graph)
    {
        printf("\nMemory allocation failed while building the graph.\n");
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < E; i++)
    {
        int src;
        int dest;
        int wt;

        if (fscanf(fp, "%d,%d,%d", &src, &dest, &wt) != 3)
        {
            printf("\nInvalid file: edge %d is not in the form '<src>,<dest>,<weight>'.\n", i + 1);
            free_weightedGraph(graph);
            fclose(fp);
            return NULL;
        }

        if (src < 0 || src >= V || dest < 0 || dest >= V)
        {
            printf("\nInvalid edge %d: vertices must be between 0 and %d.\n", i + 1, V - 1);
            free_weightedGraph(graph);
            fclose(fp);
            return NULL;
        }

        if (wt < 0)
        {
            printf("\nInvalid edge %d: weight must be non-negative.\n", i + 1);
            free_weightedGraph(graph);
            fclose(fp);
            return NULL;
        }

        add_edge_directed(graph, src, dest, wt);
    }

    fclose(fp);
    printf("\nLoaded graph from '%s' (%d vertices, %d edges).\n", path, V, E);
    return graph;
}

weightedGraph* load_weightedGraph_with_heuristic_from_csv(const char* path, int** out_h)
{
    *out_h = NULL;

    FILE* fp = fopen(path, "r");

    if (!fp)
    {
        printf("\nCould not open file '%s'. Please check the path and try again.\n", path);
        return NULL;
    }

    int V;
    int E;

    if (fscanf(fp, "%d %d", &V, &E) != 2)
    {
        printf("\nInvalid file: the first line must be '<vertices> <edges>'.\n");
        fclose(fp);
        return NULL;
    }

    if (V < 1 || V > CSV_MAX_VERTICES)
    {
        printf("\nInvalid vertex count %d (must be between 1 and %d).\n", V, CSV_MAX_VERTICES);
        fclose(fp);
        return NULL;
    }

    if (E < 0 || E > CSV_MAX_EDGES)
    {
        printf("\nInvalid edge count %d (must be between 0 and %d).\n", E, CSV_MAX_EDGES);
        fclose(fp);
        return NULL;
    }

    weightedGraph* graph = create_weightedGraph(V);

    if (!graph)
    {
        printf("\nMemory allocation failed while building the graph.\n");
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < E; i++)
    {
        int src;
        int dest;
        int wt;

        if (fscanf(fp, "%d,%d,%d", &src, &dest, &wt) != 3)
        {
            printf("\nInvalid file: edge %d is not in the form '<src>,<dest>,<weight>'.\n", i + 1);
            free_weightedGraph(graph);
            fclose(fp);
            return NULL;
        }

        if (src < 0 || src >= V || dest < 0 || dest >= V)
        {
            printf("\nInvalid edge %d: vertices must be between 0 and %d.\n", i + 1, V - 1);
            free_weightedGraph(graph);
            fclose(fp);
            return NULL;
        }

        if (wt < 0)
        {
            printf("\nInvalid edge %d: weight must be non-negative.\n", i + 1);
            free_weightedGraph(graph);
            fclose(fp);
            return NULL;
        }

        add_edge_directed(graph, src, dest, wt);
    }

    // The heuristic line is the only addition over the Dijkstra format: V
    // non-negative values, one per vertex, used by A* / Greedy as h(n).
    int* h = malloc(V * sizeof(int));

    if (!h)
    {
        printf("\nMemory allocation failed while loading heuristics.\n");
        free_weightedGraph(graph);
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < V; i++)
    {
        if (fscanf(fp, "%d", &h[i]) != 1)
        {
            printf("\nInvalid file: expected %d heuristic values on the last line.\n", V);
            free(h);
            free_weightedGraph(graph);
            fclose(fp);
            return NULL;
        }

        if (h[i] < 0)
        {
            printf("\nInvalid heuristic for vertex %d: value must be non-negative.\n", i);
            free(h);
            free_weightedGraph(graph);
            fclose(fp);
            return NULL;
        }
    }

    fclose(fp);
    *out_h = h;
    printf("\nLoaded graph from '%s' (%d vertices, %d edges) with heuristics.\n", path, V, E);
    return graph;
}

Graph* load_graph_from_csv(const char* path)
{
    FILE* fp = fopen(path, "r");

    if (!fp)
    {
        printf("\nCould not open file '%s'. Please check the path and try again.\n", path);
        return NULL;
    }

    int V;
    int E;

    if (fscanf(fp, "%d %d", &V, &E) != 2)
    {
        printf("\nInvalid file: the first line must be '<vertices> <edges>'.\n");
        fclose(fp);
        return NULL;
    }

    if (V < 1 || V > CSV_MAX_VERTICES)
    {
        printf("\nInvalid vertex count %d (must be between 1 and %d).\n", V, CSV_MAX_VERTICES);
        fclose(fp);
        return NULL;
    }

    if (E < 0 || E > CSV_MAX_EDGES)
    {
        printf("\nInvalid edge count %d (must be between 0 and %d).\n", E, CSV_MAX_EDGES);
        fclose(fp);
        return NULL;
    }

    Graph* graph = create_graph(V);

    if (!graph)
    {
        printf("\nMemory allocation failed while building the graph.\n");
        fclose(fp);
        return NULL;
    }

    for (int i = 0; i < E; i++)
    {
        int src;
        int dest;

        if (fscanf(fp, "%d,%d", &src, &dest) != 2)
        {
            printf("\nInvalid file: edge %d is not in the form '<src>,<dest>'.\n", i + 1);
            free_graph(graph);
            fclose(fp);
            return NULL;
        }

        if (src < 0 || src >= V || dest < 0 || dest >= V)
        {
            printf("\nInvalid edge %d: vertices must be between 0 and %d.\n", i + 1, V - 1);
            free_graph(graph);
            fclose(fp);
            return NULL;
        }

        add_edge_undirected(graph, src, dest);
    }

    fclose(fp);
    printf("\nLoaded graph from '%s' (%d vertices, %d edges).\n", path, V, E);
    return graph;
}

// Prints an unweighted graph as an adjacency list, one vertex per line. Each
// row reuses sll_printlist to show that vertex's neighbour list, mirroring how
// the hash-table buckets are already visualized in separate chaining.
void print_graph(const Graph* graph)
{
    if (!graph)
    {
        printf("\nNo graph to display.\n");
        return;
    }

    printf("\nAdjacency list (%d vertices):\n", graph->V);

    for (int i = 0; i < graph->V; i++)
    {
        printf("Vertex %d -> ", i);

        Node* head = graph->array[i];
        if (head == NULL)
        {
            printf("\n");
            continue;
        }
        while (head->next != NULL)
        {
            printf("%d, ", head->data);
            head = head->next;
        }

        printf("%d", head->data);

        printf("\n");
    }
}

// Prints a weighted graph as an adjacency list, one vertex per line. Mirrors the
// unweighted print_graph / sll_printlist style but walks the Edge list so each
// neighbour is shown as dest(weight).
void print_weightedGraph(const weightedGraph* graph)
{
    if (!graph)
    {
        printf("\nNo graph to display.\n");
        return;
    }

    printf("\nAdjacency list (%d vertices):\n", graph->V);

    for (int i = 0; i < graph->V; i++)
    {
        printf("Vertex %d -> ", i);

        Edge* edge = graph->array[i];
        if(edge == NULL)
        {
            printf("\n");
            continue;
        }
        while(edge->next != NULL)
        {
            printf("%d(%d), ", edge->destination, edge->weight);
            edge = edge->next;
        }

        printf("%d(%d) ", edge->destination, edge->weight);
        printf("\n");
    }
}
