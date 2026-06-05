#include "graph_io.h"
#include <stdio.h>

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
