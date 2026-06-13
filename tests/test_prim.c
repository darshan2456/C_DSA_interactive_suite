#include "graph_traversals.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_prim_simple()
{
    weightedGraph* graph = create_weightedGraph(4);
    assert(graph != NULL);

    // Create an undirected graph
    add_edge_directed(graph, 0, 1, 10);
    add_edge_directed(graph, 1, 0, 10);
    add_edge_directed(graph, 0, 2, 6);
    add_edge_directed(graph, 2, 0, 6);
    add_edge_directed(graph, 0, 3, 5);
    add_edge_directed(graph, 3, 0, 5);
    add_edge_directed(graph, 1, 3, 15);
    add_edge_directed(graph, 3, 1, 15);
    add_edge_directed(graph, 2, 3, 4);
    add_edge_directed(graph, 3, 2, 4);

    int weight = prim_mst(graph, 0);

    // MST should consist of:
    // 0 -- 3 (5)
    // 3 -- 2 (4)
    // 0 -- 1 (10)
    // Total weight = 5 + 4 + 10 = 19
    assert(weight == 19);

    free_weightedGraph(graph);
    printf("test_prim_simple passed\n");
}

void test_prim_disconnected()
{
    weightedGraph* graph = create_weightedGraph(4);
    assert(graph != NULL);

    add_edge_directed(graph, 0, 1, 10);
    add_edge_directed(graph, 1, 0, 10);
    add_edge_directed(graph, 2, 3, 5);
    add_edge_directed(graph, 3, 2, 5);

    int weight = prim_mst(graph, 0);

    // MST starting from 0 should only include 0 -- 1 (10)
    // Nodes 2 and 3 are unreachable from 0.
    assert(weight == 10);

    free_weightedGraph(graph);
    printf("test_prim_disconnected passed\n");
}

int main()
{
    test_prim_simple();
    test_prim_disconnected();

    printf("All Prim's MST tests passed\n");
    return 0;
}
