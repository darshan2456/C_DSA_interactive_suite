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

// A structure which can store the destination along with weights
typedef struct Edge
{
    int destination;
    int weight;
    struct Edge* next;
} Edge;

typedef struct
{
    int vertex;
    int distance;
} PQ_graph_node;

typedef struct
{
    int size;
    PQ_graph_node heap[HEAP_CAPACITY];
} PQ_graph;
int insert_pq_graph(PQ_graph* pq, int vertex, int distance);
bool extractTop_pq_graph(PQ_graph* pq, PQ_graph_node* result);

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

// ------------------For A* search algorithm-----------------------

int astar_solve(weightedGraph* graph, int start, int dest, int h[], int parent[]);
void astar(weightedGraph* graph, int start, int dest, int h[]);
void astar_demo(void);

// ------------------For Greedy Best-First Search-----------------------

int greedy_best_first_search_solve(weightedGraph* graph, int start, int dest, int h[], int parent[],
                                   int traversal_order[], int* traversal_len);

void greedy_best_first_search(weightedGraph* graph, int start, int dest, int h[]);
void greedy_best_first_search_demo(void);

// ------------------For Bellman-ford-----------------------
void bellman_ford_demo(void);

#endif