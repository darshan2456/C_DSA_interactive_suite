#include "graph_traversals.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void test_astar_simple_path()
{
    // Create a graph with 5 vertices
    weightedGraph* graph = create_weightedGraph(5);
    assert(graph != NULL);

    // Add edges
    add_edge_directed(graph, 0, 1, 1);
    add_edge_directed(graph, 0, 2, 4);
    add_edge_directed(graph, 1, 2, 2);
    add_edge_directed(graph, 1, 3, 5);
    add_edge_directed(graph, 2, 3, 1);
    add_edge_directed(graph, 3, 4, 3);

    // Heuristics towards node 4 (consistent and admissible)
    int h[] = {6, 5, 3, 2, 0};
    int parent[5];

    int cost = astar_solve(graph, 0, 4, h, parent);

    // Shortest path should be 0 -> 1 -> 2 -> 3 -> 4, cost = 7
    assert(cost == 7);
    assert(parent[4] == 3);
    assert(parent[3] == 2);
    assert(parent[2] == 1);
    assert(parent[1] == 0);
    assert(parent[0] == -1);

    free_weightedGraph(graph);
    printf("test_astar_simple_path passed\n");
}

void test_astar_unreachable()
{
    weightedGraph* graph = create_weightedGraph(4);
    assert(graph != NULL);

    // Node 3 is isolated
    add_edge_directed(graph, 0, 1, 2);
    add_edge_directed(graph, 1, 2, 3);

    int h[] = {4, 2, 1, 0};
    int parent[4];

    int cost = astar_solve(graph, 0, 3, h, parent);

    assert(cost == INT_MAX);

    free_weightedGraph(graph);
    printf("test_astar_unreachable passed\n");
}

void test_astar_same_source_dest()
{
    weightedGraph* graph = create_weightedGraph(3);
    assert(graph != NULL);

    add_edge_directed(graph, 0, 1, 10);
    add_edge_directed(graph, 1, 2, 5);

    int h[] = {0, 0, 0};
    int parent[3];

    int cost = astar_solve(graph, 1, 1, h, parent);

    assert(cost == 0);
    assert(parent[1] == -1);

    free_weightedGraph(graph);
    printf("test_astar_same_source_dest passed\n");
}

int main()
{
    test_astar_simple_path();
    test_astar_unreachable();
    test_astar_same_source_dest();

    printf("All A* Search tests passed\n");
    return 0;
}
