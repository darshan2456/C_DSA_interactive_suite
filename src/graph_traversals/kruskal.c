#include "graph_traversals.h"
#include "safe_input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*DSU*/

DSU* create_dsu(int n)
{
    DSU* dsu = (DSU*)malloc(sizeof(DSU));
    if (!dsu)
    {
        return NULL;
    }
    dsu->n = n;
    dsu->parent = (int*)malloc(n * sizeof(int));
    dsu->rank = (int*)malloc(n * sizeof(int));
    if (!dsu->parent || !dsu->rank)
    {
        free(dsu->parent);
        free(dsu->rank);
        free(dsu);
        return NULL;
    }
    for (int i = 0; i < n; i++)
    {
        dsu->parent[i] = i;
        dsu->rank[i] = 0;
    }
    return dsu;
}

int find_dsu(DSU* dsu, int i)
{
    if (dsu->parent[i] == i)
    {
        return i;
    }
    return dsu->parent[i] = find_dsu(dsu, dsu->parent[i]);
}

void union_dsu(DSU* dsu, int i, int j)
{
    int root_i = find_dsu(dsu, i);
    int root_j = find_dsu(dsu, j);

    if (root_i != root_j)
    {
        if (dsu->rank[root_i] < dsu->rank[root_j])
        {
            dsu->parent[root_i] = root_j;
        }
        else if (dsu->rank[root_i] > dsu->rank[root_j])
        {
            dsu->parent[root_j] = root_i;
        }
        else
        {
            dsu->parent[root_i] = root_j;
            dsu->rank[root_j]++;
        }
    }
}

void free_dsu(DSU* dsu)
{
    if (dsu)
    {
        free(dsu->parent);
        free(dsu->rank);
        free(dsu);
    }
}

/**
 * KruskalEdge - Structure to represent a weighted edge for Kruskal's algorithm.
 * @src: Source vertex.
 * @dest: Destination vertex.
 * @weight: Weight of the edge.
 */
typedef struct KruskalEdge
{
    int src;
    int dest;
    int weight;
} KruskalEdge;

/**
 * compare_edges - Comparator function for qsort to sort edges by weight.
 * @a: Pointer to the first KruskalEdge.
 * @b: Pointer to the second KruskalEdge.
 *
 * Returns a negative value if a < b, positive if a > b, and zero if equal.
 */
static int compare_edges(const void* a, const void* b)
{
    return ((KruskalEdge*)a)->weight - ((KruskalEdge*)b)->weight;
}

/**
 * kruskal_mst - Implements Kruskal's algorithm to find the Minimum Spanning Tree (MST).
 * @graph: The weighted undirected graph.
 *
 * Returns the total weight of the MST, or 0 on error/empty graph.
 */
int kruskal_mst(weightedGraph* graph)
{
    if (graph == NULL || graph->V == 0)
    {
        return 0;
    }

    int V = graph->V;
    int E = 0;

    // Count edges (assuming undirected, each edge is represented twice in adjacency list)
    for (int i = 0; i < V; i++)
    {
        Edge* curr = graph->array[i];
        while (curr)
        {
            E++;
            curr = curr->next;
        }
    }

    if (E == 0)
    {
        printf("The graph has no edges.\n");
        return 0;
    }

    KruskalEdge* edges = (KruskalEdge*)malloc(E * sizeof(KruskalEdge));
    if (!edges)
    {
        return 0;
    }

    int edge_idx = 0;
    for (int i = 0; i < V; i++)
    {
        Edge* curr = graph->array[i];
        while (curr)
        {
            edges[edge_idx].src = i;
            edges[edge_idx].dest = curr->destination;
            edges[edge_idx].weight = curr->weight;
            edge_idx++;
            curr = curr->next;
        }
    }

    clock_t start_t, end_t;
    double total_t;

    start_t = clock();

    // Sort edges by weight
    qsort(edges, E, sizeof(KruskalEdge), compare_edges);

    DSU* dsu = create_dsu(V);
    if (!dsu)
    {
        free(edges);
        return 0;
    }

    printf("\nEdges in the Minimum Spanning Tree (Kruskal's Algorithm):\n");
    printf("Source -- Destination == Weight\n");
    printf("------------------------------\n");

    int mst_weight = 0;
    int edges_count = 0;

    for (int i = 0; i < E && edges_count < V - 1; i++)
    {
        int u = edges[i].src;
        int v = edges[i].dest;

        if (find_dsu(dsu, u) != find_dsu(dsu, v))
        {
            union_dsu(dsu, u, v);
            printf("  %d    --      %d      ==   %d\n", u, v, edges[i].weight);
            mst_weight += edges[i].weight;
            edges_count++;
        }
    }

    end_t = clock();
    total_t = (double)(end_t - start_t) / CLOCKS_PER_SEC;

    if (edges_count < V - 1)
    {
        printf("\nWarning: The graph is disconnected. The result is a Minimum Spanning Forest.\n");
    }

    printf("\nTotal Weight of MST: %d\n", mst_weight);
    printf("Total CPU time taken: %f seconds\n", total_t);

    free_dsu(dsu);
    free(edges);

    return mst_weight;
}
