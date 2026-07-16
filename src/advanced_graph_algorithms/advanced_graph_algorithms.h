#ifndef ADVANCED_GRAPH_ALGORITHMS_H
#define ADVANCED_GRAPH_ALGORITHMS_H

#include "graph_traversals.h"
#include <stdbool.h>

// ------------------For Strongly Connected Components (SCC)-----------------
Graph* transpose_graph(Graph* graph);
int** find_scc_tarjan(Graph* graph, int* scc_count, int** scc_sizes);
int** find_scc_kosaraju(Graph* graph, int* scc_count, int** scc_sizes);
void free_scc_result(int** sccs, int* scc_sizes, int scc_count);
void scc_demo(void);
void tarjan_scc_demo(void);
void kosaraju_scc_demo(void);

// ------------------For Maximum Flow-----------------------------------------
int ford_fulkerson(weightedGraph* graph, int source, int sink);
int edmonds_karp(weightedGraph* graph, int source, int sink);
int dinic(weightedGraph* graph, int source, int sink);
void max_flow_demo(void);
void ford_fulkerson_demo(void);
void edmonds_karp_demo(void);
void dinic_demo(void);
bool bipartite_color(Graph* graph, int* color);

// ------------------For Bipartite Matching-----------------------------------
int max_bipartite_matching(Graph* graph, int** match_pairs, int* match_count);
int hopcroft_karp(Graph* graph, int** match_pairs, int* match_count);

// ------------------For Eulerian Path----------------------------------------
int find_eulerian_path(Graph* graph, int** path, int* path_len);

void bipartite_matching_demo(void);
void dinic_bipartite_matching_demo(void);
void hopcroft_karp_demo(void);
void eulerian_path_demo(void);
void advanced_graph_algorithms_demo(void);

#endif
