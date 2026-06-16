#include "graph_traversals.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test_kruskal_simple()
{
    weightedGraph* graph = create_weightedGraph(4);
    assert(graph != NULL);

    // Create an undirected graph by adding edges in both directions
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

    int weight = kruskal_mst(graph);

    // MST should consist of:
    // 2 -- 3 (4)
    // 0 -- 3 (5)
    // 0 -- 1 (10)
    // Total weight = 4 + 5 + 10 = 19
    assert(weight == 19);

    free_weightedGraph(graph);
    printf("test_kruskal_simple passed\n");
}

void test_kruskal_disconnected()
{
    weightedGraph* graph = create_weightedGraph(4);
    assert(graph != NULL);

    add_edge_directed(graph, 0, 1, 10);
    add_edge_directed(graph, 1, 0, 10);
    add_edge_directed(graph, 2, 3, 5);
    add_edge_directed(graph, 3, 2, 5);

    int weight = kruskal_mst(graph);

    // MST should consist of:
    // 0 -- 1 (10)
    // 2 -- 3 (5)
    // Total weight = 15
    assert(weight == 15);

    free_weightedGraph(graph);
    printf("test_kruskal_disconnected passed\n");
}

int main()
{
    test_kruskal_simple();
    test_kruskal_disconnected();

    printf("All Kruskal's MST tests passed\n");
    return 0;
}
