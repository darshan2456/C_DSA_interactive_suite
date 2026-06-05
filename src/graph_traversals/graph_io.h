#ifndef GRAPH_IO_H
#define GRAPH_IO_H
#include "graph_traversals.h"

// Loads a directed weighted graph from a CSV file for the graph algorithms.
//
// Expected format (Dijkstra):
//   line 1 : <number_of_vertices> <number_of_edges>
//   next E : <src>,<dest>,<weight>          (one directed edge per line)
//
// Returns a newly allocated weightedGraph on success (caller frees it with
// free_weightedGraph), or NULL if the file cannot be opened or its contents
// are malformed/out of range. A descriptive message is printed on failure.
weightedGraph* load_weightedGraph_from_csv(const char* path);

#endif
