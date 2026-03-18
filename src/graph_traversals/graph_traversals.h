#ifndef GRAPH_TRAVERSALS_H
#define GRAPH_TRAVERSALS_H
#include "sll.h"

void bfs_demo(void);
void dfs_demo(void);
void graph_traversals_demo(void);
typedef struct Graph
{
    int V;
    Node** array;
} Graph;

Graph* create_graph(int V);
void add_edge_undirected(Graph* graph, int src, int dest);
void free_graph(Graph* graph);
#endif