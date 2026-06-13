#ifndef DSU_H
#define DSU_H

typedef struct DSU
{
    int* parent;
    int* rank;
    int n;
} DSU;

DSU* create_dsu(int n);
int find_dsu(DSU* dsu, int i);
void union_dsu(DSU* dsu, int i, int j);
void free_dsu(DSU* dsu);

#endif
