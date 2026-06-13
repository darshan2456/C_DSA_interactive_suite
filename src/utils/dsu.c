#include "dsu.h"
#include <stdlib.h>

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
