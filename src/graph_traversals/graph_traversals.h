#ifndef GRAPH_TRAVERSALS_H
#define GRAPH_TRAVERSALS_H
#include "data_structures.h"

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

// ------------------For dijkstra's algorithm----------------------

void dijkstra_demo(void);

// A structure with can store the destination along with weights
typedef struct Edge
{
    int destination;
    int weight;
    struct Edge* next;
} Edge;

// New graph structure which stores the new type of edge node
typedef struct weightedGraph
{
    int V;
    Edge** array;
} weightedGraph;

// New function to support new graph and edge node structures
weightedGraph* create_weightedGraph(int V);
void add_edge_directed(weightedGraph* graph, int src, int dest, int wt);
int edge_insertAtEnd(Edge** head, int dest, int weight);
void free_weightedGraph(weightedGraph* graph);
int minDistance(int visited[], int dist[], int size);

#endif